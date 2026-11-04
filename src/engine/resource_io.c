/*
 * Resource file I/O of BGI 1.622 (senmomo BGI.exe), cleaned from
 * src/raw/bgi1622/native_all.c and the IDA database.
 *
 * Covers: Sys80:30 sub_489250 -> sub_466010, Sys80:31 sub_489290 ->
 * sub_466190, Sys80:32 sub_4892F0 -> sub_466390, Sys80:33 sub_489340, Sys80:34 sub_4893C0 -> sub_466B20,
 * Sys80:35 sub_489400 -> sub_466840, Sys80:36/37 sub_4657A0/sub_4657F0,
 * Sys80:39 sub_46B8F0, Sys80:3C sub_466D00, Sys80:3D sub_464F00, Sys80:3E
 * sub_489650, Sys80:3F sub_4654A0, Sys80:40 sub_489700; helpers sub_465F20,
 * sub_465E30, sub_465E10, sub_465C20, sub_465AE0, sub_465880, sub_4650E0,
 * sub_4669A0, sub_466AB0, sub_465F80, archive cache sub_406970/406AF0/
 * 406A70/406CA0/406D50/4074F0/407160.
 *
 * Status of the Rust port: see src/adr/0003 (resource I/O is being ported).
 */

/* Roots (all with a trailing backslash):
 *   primary_root   ::Buffer      game directory at startup; Sys80:3E.
 *   media_root     byte_517C18   removable-media root found by Sys80:3F;
 *                                empty when none.
 *   user_root      byte_518988   Sys80:39 (validated directory).
 * Search list: dword_566648, a singly linked list of directory names, newest
 * first (Sys80:37 prepends); used only while dword_506BE0 (Sys80:36) is set.
 */

/* sub_464EE0 */
static bool is_absolute(const char *p) { return p[0] == '\\' || p[1] == ':'; }

/* sub_4650E0(path): true for a drive path whose drive is fixed or has media
 * (DeviceIoControl CHECK_VERIFY on removable drives), and for UNC paths;
 * false for relative paths. Portable hosts: true for every resolved path. */

/* Archive entry status codes (sub_465C20 and friends). */
enum {
    E_NOT_FOUND = 0x80000010, /* archive has no such entry / not openable */
    E_NO_ENTRY  = 0x80000020, /* entry missing in an open archive (size)   */
    E_RANGE     = 0x80000030, /* -> status 2 */
    E_LENGTH    = 0x80000040, /* -> status 3 */
    E_READ      = 0x80000050, /* -> status 5 */
    E_TOO_BIG   = 0x80000060, /* -> status 6 */
};

/* sub_465880(dest, *out_size, data, len, range): decode a loaded file.
 *   "DSC FORMAT 1.00" -> DSC decode into a temporary of hdr[0x14] bytes
 *                        (> 64 MiB: 6; size mismatch: 5).
 *   "CompressedBG___" v2 (bytes 46/47 == 2/0) -> CBG v2 decode;
 *   "CompressedBG___" other -> CBG v1 decode; both produce a 16-byte header
 *                        plus width*height*(bpp==24 ? 4 : bpp/8) pixels,
 *                        decoded straight into dest when range == 0,
 *                        otherwise into a temporary; failure: 5.
 *   anything else      -> raw bytes.
 * Then the range: n = range ? range.length : total;
 *   n == 0 or n > total            -> 3
 *   (u32)(range.offset + n) > total -> 2
 *   else copy [offset, offset+n) to dest, *out_size = n, status 0. */

/* sub_465AE0(dest, *size, range) with ECX = path: not sub_4650E0 -> 1;
 * open failure -> 1; size > 64 MiB -> 6; short read -> 5; else
 * sub_465880. */

/* sub_465E30(*size, root, file, dest, range): */
int read_loose(int *size, const char *root, const char *file, void *dest, u64 range)
{
    if (is_absolute(file))
        return read_path(file, dest, size, range);           /* sub_465AE0 */
    int r = read_path(fmt("%s%s", root, file), dest, size, range);
    for (node *n = search_enabled ? search_list : 0; n && r == 1; n = n->next)
        r = read_path(fmt("%s%s\\%s", root, n->dir, file), dest, size, range);
    return r;
}

/* sub_465F20: read_loose(..., range 0) == 0 ? size : 0. */

/* sub_465C20(dest, archive_path, range) with ECX = entry name:
 * entry names of 0x60 bytes or more are fatal ("指定されたファイル名 [ %s ]
 * は%d文字を超えています", 95); size = archive_cache.size(archive_path, name)
 * (>= 0x80000000 -> E_NO_ENTRY; > 64 MiB -> E_TOO_BIG); read; decode with
 * sub_465880 mapping 2/3/5/6 to E_RANGE/E_LENGTH/E_READ/E_TOO_BIG; success
 * returns the copied size. */

/* Archive cache (dword_566768): a chain of CArchive nodes, each bound to one
 * archive file by lowercased full path (sub_406D50 opens the file into an
 * unbound node; sub_406CA0 appends a node). Entries are looked up by
 * lowercased name (sub_407160 / sub_4074F0, 128-byte records, size at
 * +100). A composite archive (Sys80:38, DCArchiveComplex) is a node whose
 * name lists member archives. */

/* sub_465F80(message): without a media root the message is a fatal script
 * error; otherwise MessageBox(title dword_566028, text dword_566338,
 * OK|CANCEL); CANCEL asks "Are you sure you want to quit?" (YES|NO, the
 * Japanese text when the locale is Japanese, sub_46FC70) and YES throws the
 * engine exit exception; then Sleep(200) and the caller retries. */

/* Sys80:30 sub_466010(file = last arg, dest = first arg, archive = middle):
 *   zero dest[0..16];
 *   r = read_whole_loose(dest, primary_root, file);              size or 0
 *   if (archive != NULL) {
 *       if (r) return r;
 *       s = archive_read(dest, primary_root + archive, file);
 *       while (s == E_NOT_FOUND || s == E_NO_ENTRY) {
 *           if (media_root ok) s = archive_read(dest, media_root + archive, file);
 *           if (still missing) media_prompt("指定されたファイル [ %s : %s ] は存在しません",
 *                                            archive, file);
 *       }
 *       return s < 0x80000000 ? s : 0;
 *   }
 *   while (!r) {
 *       r = read_whole_loose(dest, media_root, file);
 *       if (!r) media_prompt("指定されたファイル [ %s ] は存在しません", file);
 *   }
 *   return r;
 * A missing file is therefore fatal unless a media root is configured. */

/* Sys80:31 sub_466190(dest, archive, offset, length; ECX = file):
 *   r = read_loose(&n, primary_root, file, dest, (length<<32)|offset);
 *   if (r != 1) return r;                              0 success, 2/3/5/6
 *   if (archive == NULL) return read_loose(&n, media_root, file, dest, range);
 *   s = archive_read(dest, primary_root + archive, file, range);
 *   if (missing && media_root ok) s = archive_read(dest, media_root + archive, ...);
 *   return missing ? 1 : E_RANGE ? 2 : E_LENGTH ? 3 : E_READ ? 5 : E_TOO_BIG ? 6 : 0;
 * Never fatal; missing returns 1. */

/* Sys80:32 sub_4892F0 -> sub_466390(buf, len; ECX = file) (pops len, buf,
 * file): path = is_absolute(file) ? file : primary_root + file;
 * CreateFileA(GENERIC_WRITE, share 0, CREATE_ALWAYS, ARCHIVE|SEQUENTIAL_SCAN)
 * (no directories are created); WriteFile; returns written (0 when the
 * create or the write fails). The handler pushes written == len, so len 0
 * always pushes 1. */

/* Sys80:33 sub_489340(file = last, dir = first): DeleteFileA(dir ?
 * "dir\\file" : primary_root + file), no absolute check; fails on read-only
 * files. */

/* Sys80:34 sub_466B20(archive; ECX = file) (handler pops file, then archive):
 *   archive != NULL: loose exists under primary_root (sub_4669A0, with the
 *     search list) || archive_cache.exists(primary_root + archive, file)
 *     || (media_root ok && archive_cache.exists(media_root + archive, file));
 *   archive == NULL: absolute file -> exists as a non-directory; else loose
 *     exists under primary_root or media_root.
 *   "Exists" means GetFileAttributes succeeds without the DIRECTORY bit. */

/* Sys80:35 sub_466840(archive; ECX = file): size of the decoded file:
 *   loose under primary_root (sub_465F20 into a 64 MiB scratch buffer), then
 *   with an archive: primary_root + archive, then media_root + archive on
 *   E_NOT_FOUND only; without: loose under media_root. Errors give 0. */

/* Sys80:36 stores dword_506BE0; Sys80:37 prepends a copy of the directory to
 * dword_566648. */

/* Sys80:39 sub_46B8F0: an existing directory is stored in byte_518988 with a
 * trailing backslash (added only when missing); returns 1, else 0. */

/* Sys80:3C sub_466D00(file, title, text): loops until primary_root + file
 * exists as a non-directory (needs sub_4650E0(primary_root)); otherwise
 * MessageBox(title, text, OK|CANCEL); CANCEL -> quit confirmation; YES
 * returns 0 (no exit here). Returns 1 when found. */

/* Sys80:3D sub_464F00(dest, kind): kind 0 copies primary_root; kind 1
 * copies media_root when non-empty; returns 1, else 0. */

/* Sys80:3E sub_489650: an existing directory replaces primary_root as
 * "%s\\" (always appends); returns 1, else 0. */

/* Sys80:3F sub_4654A0(marker, title, text, flags): copies the marker name to
 * dword_518308, title/text to dword_566028/dword_566338 (the media-prompt
 * strings), scans drives (sub_4652D0); on success byte_518618 =
 * "<drive of media_root>\\<marker>". */

/* Sys80:40 sub_489700(file = last, archive): reads the file into a 128 KiB
 * buffer with sub_466010 (fatal "%s : %s" message when 0), then loads it as
 * a BP module (sub_4450B0); 0x80000000 is fatal. */
