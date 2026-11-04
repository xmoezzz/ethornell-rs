use anyhow::{Context, bail};
use clap::{Parser, Subcommand};
use ethornell_archive::{
    ArchiveFormat, MagicKind, ResourceManager, archive_paths, detect_archive_format_from_bytes,
    detect_file_magic, detect_magic, extract_archive_with_options, extract_game_archives,
    read_archive_index, read_entry_raw, scan_game_root, walk_files,
};
use ethornell_core::{GameRoot, init_tracing};
use ethornell_image::{decode_cbg_to_png, decode_image, probe_image, write_rgba_png};
use ethornell_script::{
    BpInstruction,
    bcs::{BcsCommand, BcsValue, parse_bcs},
    calls::{
        CallKey, CallSite, DISPATCH_GROUPS, infer_program_call_arg_counts, instruction_call_key,
        registered_call_summary, scan_program_calls, summarize_call_sites,
        summarize_inferred_arg_counts,
    },
    decompile::{DecompileOptions, decompile_bcs, decompile_bp},
    detect_script_format, disassemble_bp, disassemble_file, parse_bp_program,
};
use serde::Serialize;
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};

#[derive(Debug, Parser)]
#[command(name = "ethornell")]
#[command(about = "Rust Ethornell / BURIKO General Interpreter runtime tools")]
struct Cli {
    #[command(subcommand)]
    command: Option<Command>,
    /// Game folder to play. Without a command or a folder (a double-click on
    /// the program), a folder dialog asks for one; a folder dropped on the
    /// program arrives here.
    game: Option<PathBuf>,
    /// Display text: auto, cp932 (Shift-JIS), or gbk. Overrides ETHORNELL_TEXT_ENCODING.
    #[arg(long, global = true)]
    text_encoding: Option<ethornell_app::TextEncoding>,
}

#[derive(Debug, Subcommand)]
enum Command {
    Scan {
        #[arg(long)]
        game: PathBuf,
    },
    Info {
        file: PathBuf,
    },
    Disasm {
        file: Option<PathBuf>,
        #[arg(long)]
        game: Option<PathBuf>,
        #[arg(long)]
        json: bool,
        #[arg(long)]
        limit: Option<usize>,
        #[arg(long, default_value_t = 10)]
        limit_files: usize,
    },
    Decompile {
        file: Option<PathBuf>,
        #[arg(long)]
        game: Option<PathBuf>,
        #[arg(long)]
        archive: Option<String>,
        #[arg(long)]
        script: Option<String>,
        #[arg(long)]
        limit: Option<usize>,
        #[arg(long)]
        show_stack: bool,
    },
    CallCatalog {
        #[arg(long)]
        game: PathBuf,
        #[arg(long)]
        json: bool,
        #[arg(long)]
        unknown_only: bool,
        #[arg(long)]
        all_registered: bool,
        #[arg(long)]
        generic_only: bool,
        #[arg(long)]
        limit_scripts: Option<usize>,
    },
    CallSites {
        #[arg(long)]
        game: PathBuf,
        #[arg(long)]
        archive: Option<String>,
        #[arg(long)]
        script: Option<String>,
        #[arg(long, value_parser = parse_hex_u8)]
        group: u8,
        #[arg(long, value_parser = parse_hex_u16)]
        id: u16,
        #[arg(long, default_value_t = 8)]
        before: usize,
        #[arg(long, default_value_t = 8)]
        after: usize,
        #[arg(long)]
        limit: Option<usize>,
        #[arg(long)]
        decompiled: bool,
    },
    Run {
        #[arg(long)]
        game: PathBuf,
        #[arg(long)]
        trace: bool,
        #[arg(
            long,
            help = "Abort when a native selector is explicitly Stub or has not passed selector-by-selector target audit"
        )]
        fail_on_stub: bool,
        #[arg(long)]
        force_text_test: bool,
        #[arg(long)]
        headless: bool,
        #[arg(long)]
        script: Option<String>,
    },
    ArcList {
        archive: Option<PathBuf>,
        #[arg(long)]
        game: Option<PathBuf>,
        #[arg(long)]
        limit: Option<usize>,
    },
    Extract {
        archive: Option<PathBuf>,
        #[arg(long)]
        game: Option<PathBuf>,
        #[arg(long)]
        out: PathBuf,
        #[arg(long)]
        raw: bool,
    },
    DumpMagic {
        #[arg(long)]
        game: Option<PathBuf>,
        #[arg(long)]
        dir: Option<PathBuf>,
    },
    ResourceList {
        #[arg(long)]
        game: PathBuf,
        #[arg(long)]
        limit: Option<usize>,
    },
    ResourceFind {
        #[arg(long)]
        game: PathBuf,
        #[arg(long)]
        contains: String,
    },
    ResourceRead {
        #[arg(long)]
        game: PathBuf,
        #[arg(long)]
        archive: Option<String>,
        #[arg(long)]
        name: String,
        #[arg(long)]
        out: PathBuf,
        #[arg(long)]
        raw: bool,
    },
    ImageInfo {
        file: PathBuf,
    },
    DecodeImage {
        file: PathBuf,
        #[arg(long)]
        out: PathBuf,
    },
    DecodeImages {
        #[arg(long)]
        game: Option<PathBuf>,
        #[arg(long)]
        dir: Option<PathBuf>,
        #[arg(long)]
        out: PathBuf,
        #[arg(long)]
        limit: Option<usize>,
    },
    ScriptInfo {
        file: Option<PathBuf>,
        #[arg(long)]
        game: Option<PathBuf>,
        #[arg(long)]
        script: Option<String>,
        #[arg(long)]
        limit: Option<usize>,
    },
    BcsCatalog {
        #[arg(long)]
        game: PathBuf,
        #[arg(long)]
        json: bool,
    },
    AudioScan {
        #[arg(long)]
        game: Option<PathBuf>,
        #[arg(long)]
        dir: Option<PathBuf>,
        #[arg(long)]
        limit: Option<usize>,
    },
    AudioInfo {
        #[arg(long)]
        game: PathBuf,
        #[arg(long)]
        name: String,
    },
    PlayAudio {
        #[arg(long)]
        game: PathBuf,
        #[arg(long)]
        name: String,
    },
    VmTrace {
        #[arg(long)]
        game: PathBuf,
        #[arg(long)]
        archive: Option<String>,
        #[arg(long)]
        script: String,
        #[arg(long, default_value_t = 10000)]
        max_steps: usize,
        #[arg(
            long,
            help = "Abort when a native selector is explicitly Stub or has not passed selector-by-selector target audit"
        )]
        fail_on_stub: bool,
        #[arg(long)]
        quiet_trace: bool,
    },
    TextTest {
        #[arg(long, default_value = "こんにちは\nHello Ethornell")]
        text: String,
    },
    ViewImage {
        file: PathBuf,
    },
    ViewResourceImage {
        #[arg(long)]
        game: PathBuf,
        #[arg(long)]
        name: String,
    },
}

#[derive(Debug, Serialize)]
struct FileInfo {
    path: PathBuf,
    size: u64,
    magic: MagicKind,
    archive_format: ArchiveFormat,
    script_format: ethornell_script::ScriptFormat,
    image: ethornell_image::ImageInfo,
}

#[derive(Debug, Default)]
struct MagicStats {
    counts: BTreeMap<&'static str, usize>,
    samples: BTreeMap<&'static str, Vec<String>>,
}

impl MagicStats {
    fn add(&mut self, kind: MagicKind, path: impl Into<String>) {
        let label = kind.label();
        *self.counts.entry(label).or_default() += 1;
        let samples = self.samples.entry(label).or_default();
        if samples.len() < 20 {
            samples.push(path.into());
        }
    }

    fn print(&self) {
        let order = [
            "CompressedBG___",
            "DSC FORMAT 1.00",
            "BurikoCompiledScriptVer1.00",
            "._bp",
            "OggS",
            "RIFF/WAVE",
            "BSE",
            "Unknown",
        ];
        for label in order {
            let count = self.counts.get(label).copied().unwrap_or(0);
            println!("{label}: {count}");
            if let Some(samples) = self.samples.get(label) {
                for sample in samples {
                    println!("  {sample}");
                }
            }
        }
        for (label, count) in &self.counts {
            if !order.contains(label) {
                println!("{label}: {count}");
                if let Some(samples) = self.samples.get(label) {
                    for sample in samples {
                        println!("  {sample}");
                    }
                }
            }
        }
    }
}

/// Plays a game without a command line: the folder given (dropped on the
/// program) or one chosen in a dialog. Errors are shown in a dialog too, as
/// there may be no terminal to print them.
fn play(
    game: Option<PathBuf>,
    text_encoding: Option<ethornell_app::TextEncoding>,
) -> anyhow::Result<()> {
    let Some(game) = game.or_else(ethornell_app::choose_game_folder) else {
        return Ok(());
    };
    let result = GameRoot::new(game).map_err(anyhow::Error::from).and_then(|game_root| {
        ethornell_app::run(ethornell_app::AppConfig {
            text_encoding,
            game_root,
            trace: false,
            fail_on_stub: false,
            force_text_test: false,
            headless: false,
            script: None,
        })
        .map_err(anyhow::Error::from)
    });
    if let Err(error) = &result {
        ethornell_app::show_error("Ethornell", &format!("{error:#}"));
    }
    result
}

fn main() -> anyhow::Result<()> {
    init_tracing();
    let cli = Cli::parse();
    if cli.game.is_some() && cli.command.is_some() {
        bail!("a game folder and a subcommand cannot be combined");
    }
    let Some(command) = cli.command else {
        return play(cli.game, cli.text_encoding);
    };

    match command {
        Command::Scan { game } => {
            let root = GameRoot::new(game)?;
            let report = scan_game_root(&root)?;
            println!("{}", serde_json::to_string_pretty(&report)?);
        }
        Command::Info { file } => {
            let bytes = std::fs::read(&file).with_context(|| format!("read {}", file.display()))?;
            let metadata = std::fs::metadata(&file)?;
            let info = FileInfo {
                path: file.clone(),
                size: metadata.len(),
                magic: detect_file_magic(&file, &bytes),
                archive_format: detect_archive_format_from_bytes(&bytes),
                script_format: detect_script_format(&file, &bytes),
                image: probe_image(&bytes)?,
            };
            println!("{}", serde_json::to_string_pretty(&info)?);
        }
        Command::Disasm {
            file,
            game,
            json,
            limit,
            limit_files,
        } => run_disasm(file, game, json, limit, limit_files)?,
        Command::Decompile {
            file,
            game,
            archive,
            script,
            limit,
            show_stack,
        } => run_decompile(file, game, archive, script, limit, show_stack)?,
        Command::CallCatalog {
            game,
            json,
            unknown_only,
            all_registered,
            generic_only,
            limit_scripts,
        } => run_call_catalog(
            game,
            json,
            unknown_only,
            all_registered,
            generic_only,
            limit_scripts,
        )?,
        Command::CallSites {
            game,
            archive,
            script,
            group,
            id,
            before,
            after,
            limit,
            decompiled,
        } => run_call_sites(
            game,
            archive.as_deref(),
            script.as_deref(),
            group,
            id,
            before,
            after,
            limit,
            decompiled,
        )?,
        Command::Run {
            game,
            trace,
            fail_on_stub,
            force_text_test,
            headless,
            script,
        } => {
            let game_root = GameRoot::new(game)?;
            ethornell_app::run(ethornell_app::AppConfig {
                text_encoding: cli.text_encoding,
                game_root,
                trace,
                fail_on_stub,
                force_text_test,
                headless,
                script,
            })?;
        }
        Command::ArcList {
            archive,
            game,
            limit,
        } => run_arc_list(archive, game, limit)?,
        Command::Extract {
            archive,
            game,
            out,
            raw,
        } => run_extract(archive, game, &out, raw)?,
        Command::DumpMagic { game, dir } => run_dump_magic(game, dir)?,
        Command::ResourceList { game, limit } => run_resource_list(game, limit)?,
        Command::ResourceFind { game, contains } => run_resource_find(game, &contains)?,
        Command::ResourceRead {
            game,
            archive,
            name,
            out,
            raw,
        } => run_resource_read(game, archive.as_deref(), &name, &out, raw)?,
        Command::ImageInfo { file } => run_image_info(&file)?,
        Command::DecodeImage { file, out } => run_decode_image(&file, &out)?,
        Command::DecodeImages {
            game,
            dir,
            out,
            limit,
        } => run_decode_images(game, dir, &out, limit)?,
        Command::ScriptInfo {
            file,
            game,
            script,
            limit,
        } => run_script_info(file, game, script, limit)?,
        Command::BcsCatalog { game, json } => run_bcs_catalog(game, json)?,
        Command::AudioScan { game, dir, limit } => run_audio_scan(game, dir, limit)?,
        Command::AudioInfo { game, name } => run_audio_info(game, &name)?,
        Command::PlayAudio { game, name } => run_play_audio(game, &name)?,
        Command::VmTrace {
            game,
            archive,
            script,
            max_steps,
            fail_on_stub,
            quiet_trace,
        } => run_vm_trace(
            game,
            archive.as_deref(),
            &script,
            max_steps,
            fail_on_stub,
            quiet_trace,
        )?,
        Command::TextTest { text } => run_text_test(&text)?,
        Command::ViewImage { file } => run_view_image(&file)?,
        Command::ViewResourceImage { game, name } => run_view_resource_image(game, &name)?,
    }

    Ok(())
}

fn parse_hex_u8(value: &str) -> Result<u8, String> {
    let text = value
        .strip_prefix("0x")
        .or_else(|| value.strip_prefix("0X"))
        .unwrap_or(value);
    u8::from_str_radix(text, 16).map_err(|err| err.to_string())
}

fn parse_hex_u16(value: &str) -> Result<u16, String> {
    let text = value
        .strip_prefix("0x")
        .or_else(|| value.strip_prefix("0X"))
        .unwrap_or(value);
    u16::from_str_radix(text, 16).map_err(|err| err.to_string())
}

fn run_arc_list(
    archive: Option<PathBuf>,
    game: Option<PathBuf>,
    limit: Option<usize>,
) -> anyhow::Result<()> {
    if let Some(game) = game {
        let root = GameRoot::new(game)?;
        let archives = archive_paths(&root)?;
        let mut total_entries = 0usize;
        println!("archives: {}", archives.len());
        for path in archives {
            let index = read_archive_index(&path)?;
            total_entries += index.entries.len();
            println!(
                "{} {:?} entries={}",
                path.display(),
                index.format,
                index.entries.len()
            );
            for (i, entry) in index
                .entries
                .iter()
                .take(limit.unwrap_or(usize::MAX))
                .enumerate()
            {
                println!(
                    "  #{i:04} off=0x{:08X} size={} {}",
                    entry.offset, entry.packed_size, entry.name
                );
            }
        }
        println!("total_entries: {total_entries}");
        return Ok(());
    }

    let archive = archive.context("arc-list requires <archive.arc> or --game <dir>")?;
    let index = read_archive_index(&archive)?;
    println!(
        "{} {:?} entries={}",
        index.path.display(),
        index.format,
        index.entries.len()
    );
    for (i, entry) in index
        .entries
        .iter()
        .take(limit.unwrap_or(usize::MAX))
        .enumerate()
    {
        println!(
            "#{i:04} off=0x{:08X} size={} flags=0x{:08X} method={} {}",
            entry.offset,
            entry.packed_size,
            entry.flags,
            entry.method.as_deref().unwrap_or("-"),
            entry.name
        );
    }
    Ok(())
}

fn run_extract(
    archive: Option<PathBuf>,
    game: Option<PathBuf>,
    out: &Path,
    raw: bool,
) -> anyhow::Result<()> {
    if let Some(game) = game {
        let root = GameRoot::new(game)?;
        let summary = extract_game_archives(&root, out, raw)?;
        println!("{}", serde_json::to_string_pretty(&summary)?);
        return Ok(());
    }

    let archive = archive.context("extract requires <archive.arc> or --game <dir>")?;
    let summary = extract_archive_with_options(&archive, out, raw)?;
    println!("{}", serde_json::to_string_pretty(&summary)?);
    Ok(())
}

fn run_dump_magic(game: Option<PathBuf>, dir: Option<PathBuf>) -> anyhow::Result<()> {
    let mut stats = MagicStats::default();
    match (game, dir) {
        (Some(game), None) => {
            let root = GameRoot::new(game)?;
            for archive in archive_paths(&root)? {
                let index = read_archive_index(&archive)?;
                for entry in &index.entries {
                    let raw = read_entry_raw(&archive, entry)?;
                    let kind = detect_entry_magic(entry.name.as_str(), &raw);
                    stats.add(kind, format!("{}:{}", archive.display(), entry.name));
                }
            }
        }
        (None, Some(dir)) => {
            for path in walk_files(&dir)? {
                let bytes = read_header(&path)?;
                let kind = detect_file_magic(&path, &bytes);
                stats.add(kind, path.display().to_string());
            }
        }
        _ => bail!("dump-magic requires exactly one of --game <dir> or --dir <dir>"),
    }
    stats.print();
    Ok(())
}

fn run_resource_list(game: PathBuf, limit: Option<usize>) -> anyhow::Result<()> {
    let manager = ResourceManager::open_game(&game)?;
    println!(
        "archives: {} resources: {}",
        manager.archives().archives.len(),
        manager.list().len()
    );
    for (i, entry) in manager
        .list()
        .into_iter()
        .take(limit.unwrap_or(usize::MAX))
        .enumerate()
    {
        println!(
            "#{i:04} {}:{} size={} flags=0x{:08X}",
            entry.archive_path.display(),
            entry.entry_name,
            entry.packed_size,
            entry.flags
        );
    }
    Ok(())
}

fn run_resource_find(game: PathBuf, contains: &str) -> anyhow::Result<()> {
    let manager = ResourceManager::open_game(&game)?;
    let matches = manager.find_all(contains);
    println!("matches: {}", matches.len());
    for (i, entry) in matches.iter().take(200).enumerate() {
        println!(
            "#{i:04} {}:{} size={} flags=0x{:08X}",
            entry.archive_path.display(),
            entry.entry_name,
            entry.packed_size,
            entry.flags
        );
    }
    Ok(())
}

fn run_resource_read(
    game: PathBuf,
    archive: Option<&str>,
    name: &str,
    out: &Path,
    raw: bool,
) -> anyhow::Result<()> {
    let manager = ResourceManager::open_game(&game)?;
    let data = match (archive, raw) {
        (Some(archive), false) => manager.read_decoded_from_archive(archive, name)?,
        (Some(archive), true) => {
            let entry = manager
                .find_in_archive(archive, name)
                .with_context(|| format!("{archive}:{name} not found"))?;
            manager.read_by_entry_raw(&entry)?
        }
        (None, true) => manager.read_raw(name)?,
        (None, false) => manager.read_decoded(name)?,
    };
    if let Some(parent) = out.parent() {
        std::fs::create_dir_all(parent)?;
    }
    std::fs::write(out, &data)?;
    println!("wrote {} bytes to {}", data.len(), out.display());
    Ok(())
}

fn run_image_info(file: &Path) -> anyhow::Result<()> {
    let bytes = std::fs::read(file)?;
    println!("{}", serde_json::to_string_pretty(&probe_image(&bytes)?)?);
    Ok(())
}

fn run_decode_image(file: &Path, out: &Path) -> anyhow::Result<()> {
    let bytes = std::fs::read(file)?;
    if detect_magic(&bytes) == MagicKind::CompressedBg {
        decode_cbg_to_png(&bytes, out)?;
    } else {
        let image = decode_image(&bytes)?;
        write_rgba_png(&image, out)?;
    }
    println!("wrote {}", out.display());
    Ok(())
}

fn run_decode_images(
    game: Option<PathBuf>,
    dir: Option<PathBuf>,
    out: &Path,
    limit: Option<usize>,
) -> anyhow::Result<()> {
    std::fs::create_dir_all(out)?;
    let mut decoded = 0usize;
    let mut errors = Vec::new();
    match (game, dir) {
        (Some(game), None) => {
            let manager = ResourceManager::open_game(&game)?;
            for entry in manager.list() {
                if decoded >= limit.unwrap_or(usize::MAX) {
                    break;
                }
                let data = match manager.read_by_entry_decoded(&entry) {
                    Ok(data) => data,
                    Err(err) => {
                        errors.push(format!("{}: {err}", entry.entry_name));
                        continue;
                    }
                };
                if detect_magic(&data) != MagicKind::CompressedBg {
                    continue;
                }
                let output = out.join(format!(
                    "{}.png",
                    sanitize_output_name(&format!(
                        "{}__{}",
                        entry
                            .archive_path
                            .file_stem()
                            .and_then(|s| s.to_str())
                            .unwrap_or("archive"),
                        entry.entry_name
                    ))
                ));
                match decode_cbg_to_png(&data, &output) {
                    Ok(()) => {
                        decoded += 1;
                        println!("decoded {}", output.display());
                    }
                    Err(err) => errors.push(format!("{}: {err}", entry.entry_name)),
                }
            }
        }
        (None, Some(dir)) => {
            for path in walk_files(&dir)? {
                if decoded >= limit.unwrap_or(usize::MAX) {
                    break;
                }
                let data = std::fs::read(&path)?;
                if detect_file_magic(&path, &data) != MagicKind::CompressedBg {
                    continue;
                }
                let output = out.join(format!(
                    "{}.png",
                    sanitize_output_name(
                        &path
                            .strip_prefix(&dir)
                            .unwrap_or(&path)
                            .display()
                            .to_string()
                    )
                ));
                match decode_cbg_to_png(&data, &output) {
                    Ok(()) => {
                        decoded += 1;
                        println!("decoded {}", output.display());
                    }
                    Err(err) => errors.push(format!("{}: {err}", path.display())),
                }
            }
        }
        _ => bail!("decode-images requires exactly one of --game <dir> or --dir <dir>"),
    }
    println!("decoded: {decoded}");
    if !errors.is_empty() {
        println!("errors: {}", errors.len());
        for err in errors.iter().take(20) {
            println!("  {err}");
        }
    }
    Ok(())
}

fn sanitize_output_name(name: &str) -> String {
    name.chars()
        .map(|ch| match ch {
            '/' | '\\' | ':' | '*' | '?' | '"' | '<' | '>' | '|' => '_',
            ch if ch.is_control() => '_',
            ch => ch,
        })
        .collect()
}

fn run_script_info(
    file: Option<PathBuf>,
    game: Option<PathBuf>,
    script: Option<String>,
    limit: Option<usize>,
) -> anyhow::Result<()> {
    if let Some(file) = file {
        let bytes = std::fs::read(&file)?;
        if let Some(bcs) = parse_bcs(&bytes) {
            print_bcs_summary(&file.display().to_string(), &bytes, &bcs, limit);
            return Ok(());
        }
        let program = parse_bp_program(Some(file.display().to_string()), &bytes);
        print_script_summary(&file.display().to_string(), &bytes, &program);
        return Ok(());
    }
    let game = game.context("script-info requires <file> or --game <dir>")?;
    let manager = ResourceManager::open_game(&game)?;
    if let Some(script) = script {
        let bytes = manager.read_decoded(&script)?;
        if let Some(bcs) = parse_bcs(&bytes) {
            print_bcs_summary(&script, &bytes, &bcs, limit);
            return Ok(());
        }
        let program = parse_bp_program(Some(script.clone()), &bytes);
        print_script_summary(&script, &bytes, &program);
        return Ok(());
    }
    let scripts: Vec<_> = manager
        .list()
        .into_iter()
        .filter(|entry| entry.entry_name.to_ascii_lowercase().ends_with("._bp"))
        .collect();
    println!("bp_files: {}", scripts.len());
    for entry in scripts.into_iter().take(limit.unwrap_or(usize::MAX)) {
        match manager.read_by_entry_decoded(&entry) {
            Ok(bytes) => {
                let program = parse_bp_program(Some(entry.entry_name.clone()), &bytes);
                print_script_summary(
                    &format!("{}:{}", entry.archive_path.display(), entry.entry_name),
                    &bytes,
                    &program,
                );
            }
            Err(err) => println!(
                "{}:{} error={err}",
                entry.archive_path.display(),
                entry.entry_name
            ),
        }
    }
    Ok(())
}

#[derive(Debug, Serialize)]
struct BcsOpcodeSummary {
    opcode: String,
    name: Option<&'static str>,
    count: usize,
    scripts: Vec<String>,
}

fn run_bcs_catalog(game: PathBuf, json: bool) -> anyhow::Result<()> {
    let manager = ResourceManager::open_game(&game)?;
    let mut scripts_scanned = 0usize;
    let mut commands_scanned = 0usize;
    let mut summaries: BTreeMap<u32, (usize, BTreeSet<String>)> = BTreeMap::new();

    for entry in manager.list() {
        let Ok(bytes) = manager.read_by_entry_decoded(&entry) else {
            continue;
        };
        let Some(program) = parse_bcs(&bytes) else {
            continue;
        };
        scripts_scanned += 1;
        commands_scanned += program.commands.len();
        let label = format!("{}:{}", entry.archive_path.display(), entry.entry_name);
        for command in program.commands {
            let (count, scripts) = summaries.entry(command.opcode).or_default();
            *count += 1;
            if scripts.len() < 12 {
                scripts.insert(label.clone());
            }
        }
    }

    let rows = summaries
        .into_iter()
        .map(|(opcode, (count, scripts))| BcsOpcodeSummary {
            opcode: format!("0x{opcode:03X}"),
            name: ethornell_script::bcs::command_name(opcode),
            count,
            scripts: scripts.into_iter().collect(),
        })
        .collect::<Vec<_>>();

    if json {
        println!(
            "{}",
            serde_json::to_string_pretty(&serde_json::json!({
                "scripts_scanned": scripts_scanned,
                "commands_scanned": commands_scanned,
                "opcodes": rows,
            }))?
        );
    } else {
        println!("bcs_scripts={scripts_scanned} commands={commands_scanned}");
        for row in rows {
            println!(
                "{} count={} name={} samples={}",
                row.opcode,
                row.count,
                row.name.unwrap_or("<unknown>"),
                row.scripts.join(", ")
            );
        }
    }
    Ok(())
}

fn print_script_summary(label: &str, bytes: &[u8], program: &ethornell_script::BpProgram) {
    let mut calls: BTreeMap<String, usize> = BTreeMap::new();
    for inst in program.instructions.iter() {
        if let Some(name) = inst.known_call {
            *calls.entry(name.to_string()).or_default() += 1;
        }
    }
    println!(
        "== {label} bytes={} instructions={} functions={} strings={} labels={} known_calls={:?}",
        bytes.len(),
        program.instructions.len(),
        program.functions.len(),
        program.strings.len(),
        program.labels.len(),
        calls
    );
    for warning in program.warnings.iter().take(10) {
        println!("  warning: {warning}");
    }
}

fn print_bcs_summary(
    label: &str,
    bytes: &[u8],
    program: &ethornell_script::bcs::BcsProgram,
    limit: Option<usize>,
) {
    let mut opcodes: BTreeMap<String, usize> = BTreeMap::new();
    for command in &program.commands {
        let label = command
            .name
            .map(str::to_string)
            .unwrap_or_else(|| format!("0x{:X}", command.opcode));
        *opcodes.entry(label).or_default() += 1;
    }
    println!(
        "== {label} bytes={} bcs code=0x{:X}..0x{:X} namespaces={} subs={} commands={} opcodes={:?}",
        bytes.len(),
        program.code_start,
        program.code_end,
        program.namespaces.len(),
        program.subs.len(),
        program.commands.len(),
        opcodes
    );
    for warning in program.warnings.iter().take(10) {
        println!("  warning: {warning}");
    }
    let sub_limit = limit.unwrap_or(usize::MAX);
    for (index, sub) in program.subs.iter().take(sub_limit).enumerate() {
        println!("  sub #{index:04} addr=0x{:X} name={}", sub.addr, sub.name);
    }
    if program.subs.len() > sub_limit {
        println!("  ... {} more subs", program.subs.len() - sub_limit);
    }
    if let Some(limit) = limit {
        for (index, command) in program.commands.iter().take(limit).enumerate() {
            println!(
                "  #{index:04} off=0x{:X} op=0x{:X} {} args=[{}] strings={:?}",
                command.file_offset,
                command.opcode,
                command.name.unwrap_or("unknown"),
                command
                    .args
                    .iter()
                    .map(format_bcs_value)
                    .collect::<Vec<_>>()
                    .join(", "),
                command
                    .string_refs
                    .iter()
                    .map(|string_ref| format!("0x{:X}:{}", string_ref.offset, string_ref.text))
                    .collect::<Vec<_>>()
            );
        }
    }
}

fn format_bcs_value(value: &BcsValue) -> String {
    match value {
        BcsValue::Int(value) => value.to_string(),
        BcsValue::Addr(value) => format!("addr:0x{value:X}"),
        BcsValue::BaseOffset(value) => format!("base:{value}"),
        BcsValue::MemoryAddr(value) => format!("mem:0x{value:X}"),
        BcsValue::Arg2 => "arg2".to_string(),
        BcsValue::Str(value) => format!("{value:?}"),
        BcsValue::Line { file, line } => format!("line:{file}:{line}"),
        BcsValue::Mul(left, right) => {
            format!("({}*{})", format_bcs_value(left), format_bcs_value(right))
        }
        BcsValue::CheckNote(value) => format!("note:{}", format_bcs_value(value)),
    }
}

fn run_audio_scan(
    game: Option<PathBuf>,
    dir: Option<PathBuf>,
    limit: Option<usize>,
) -> anyhow::Result<()> {
    let mut stats = MagicStats::default();
    let mut bw_samples = Vec::new();
    match (game, dir) {
        (Some(game), None) => {
            let manager = ResourceManager::open_game(game)?;
            for entry in manager.list().into_iter().take(limit.unwrap_or(usize::MAX)) {
                let raw = manager.read_by_entry_raw(&entry)?;
                let audio = ethornell_audio::probe_audio(&raw);
                if matches!(
                    audio.kind,
                    ethornell_audio::AudioKind::BurikoWaveBoxOgg
                        | ethornell_audio::AudioKind::BurikoWaveBoxUnknown
                ) && bw_samples.len() < 20
                {
                    bw_samples.push(format!(
                        "{}:{} {:?}",
                        entry.archive_path.display(),
                        entry.entry_name,
                        audio
                    ));
                }
                stats.add(
                    detect_entry_magic(&entry.entry_name, &raw),
                    format!("{}:{}", entry.archive_path.display(), entry.entry_name),
                );
            }
        }
        (None, Some(dir)) => {
            for path in walk_files(&dir)? {
                let header = read_header(&path)?;
                stats.add(
                    detect_file_magic(&path, &header),
                    path.display().to_string(),
                );
            }
        }
        _ => bail!("audio-scan requires exactly one of --game <dir> or --dir <dir>"),
    }
    stats.print();
    println!("buriko_wavebox_samples: {}", bw_samples.len());
    for sample in bw_samples {
        println!("  {sample}");
    }
    Ok(())
}

fn run_audio_info(game: PathBuf, name: &str) -> anyhow::Result<()> {
    let manager = ResourceManager::open_game(&game)?;
    let raw = manager.read_raw(name)?;
    let decoded = manager.read_decoded(name).unwrap_or_else(|_| raw.clone());
    let raw_info = ethornell_audio::probe_audio(&raw);
    let decoded_info = ethornell_audio::probe_audio(&decoded);
    println!(
        "{}",
        serde_json::to_string_pretty(&serde_json::json!({
            "name": name,
            "raw_len": raw.len(),
            "decoded_len": decoded.len(),
            "raw": raw_info,
            "decoded": decoded_info,
            "decoded_ogg_payload_len": ethornell_audio::unwrap_buriko_wave_ogg(&decoded).map(|p| p.len()),
        }))?
    );
    Ok(())
}

fn run_play_audio(game: PathBuf, name: &str) -> anyhow::Result<()> {
    let manager = ResourceManager::open_game(&game)?;
    let data = manager.read_decoded(name)?;
    let kind = detect_magic(&data);
    if let Some(payload) = ethornell_audio::unwrap_buriko_wave_ogg(&data) {
        println!(
            "unwrapped BurikoWaveBox Ogg payload: {} bytes",
            payload.len()
        );
        bail!("byte-stream playback is not wired yet; unwrap succeeded")
    }
    match kind {
        MagicKind::Ogg | MagicKind::RiffWave => {
            bail!("archive byte-stream playback is not wired yet; decoded kind={kind:?}")
        }
        _ => bail!("unsupported audio resource {name}: decoded kind={kind:?}"),
    }
}

fn run_vm_trace(
    game: PathBuf,
    archive: Option<&str>,
    script: &str,
    max_steps: usize,
    fail_on_stub: bool,
    quiet_trace: bool,
) -> anyhow::Result<()> {
    let manager = ResourceManager::open_game(&game)?;
    let bytes = if let Some(archive) = archive {
        manager.read_decoded_from_archive(archive, script)?
    } else {
        manager.read_decoded(script)?
    };
    let program = parse_bp_program(Some(script.to_string()), &bytes);
    let mut vm = ethornell_vm::Vm::new();
    let mut api = VmTraceApi {
        manager: ResourceManager::open_game(&game)?,
        fallback: ethornell_vm::TraceApi,
    };
    let report = vm.run(
        &program,
        &mut api,
        &ethornell_vm::VmRunOptions {
            max_steps,
            trace: !quiet_trace,
            fail_on_stub,
            collect_diagnostics: true,
        },
    );
    println!("steps={}", report.steps);
    println!("pc={}", report.pc);
    println!("offset={:?}", report.offset);
    println!("stop_reason={:?}", report.stop_reason);
    println!("thread={:#?}", report.thread);
    println!("stack={}", vm.stack.len());
    println!("calls={:#?}", report.calls);
    println!("stubs={:#?}", report.stubs);
    if fail_on_stub && !report.recent_trace.is_empty() {
        println!("recent_trace:");
        for line in report.recent_trace {
            println!("  {line}");
        }
    }
    Ok(())
}

struct VmTraceApi {
    manager: ResourceManager,
    fallback: ethornell_vm::TraceApi,
}

impl ethornell_vm::SysApi for VmTraceApi {
    fn call_sys(
        &mut self,
        call: &mut ethornell_vm::NativeCallFrame,
    ) -> ethornell_vm::VmResult<ethornell_vm::Value> {
        let (group, id) = (call.group(), call.id());
        let stack = call.args_mut();
        match (group, id) {
            (0x80, 0x40) => {
                let file = pop_vm_string(stack).unwrap_or_else(|| "<unknown>".into());
                let archive = pop_vm_string(stack).unwrap_or_else(|| "<unknown>".into());
                let bytes = self
                    .manager
                    .read_decoded_from_archive(&archive, &file)
                    .map_err(|err| ethornell_vm::VmError::Runtime(err.to_string()))?;
                let program = parse_bp_program(Some(format!("{archive}:{file}")), &bytes);
                return Ok(ethornell_vm::Value::Program(std::sync::Arc::new(program)));
            }
            (0x80, 0x44) => {
                for _ in 0..3 {
                    let _ = stack.pop();
                }
                let file = pop_vm_string(stack).unwrap_or_else(|| "<unknown>".into());
                let archive = pop_vm_string(stack).unwrap_or_else(|| "<unknown>".into());
                let bytes = self
                    .manager
                    .read_decoded_from_archive(&archive, &file)
                    .map_err(|err| ethornell_vm::VmError::Runtime(err.to_string()))?;
                let program = parse_bp_program(Some(format!("{archive}:{file}")), &bytes);
                return Ok(ethornell_vm::Value::Program(std::sync::Arc::new(program)));
            }
            (0x80, 0x34) => {
                let file = pop_vm_string(stack).unwrap_or_else(|| "<unknown>".into());
                let archive = pop_vm_string(stack).unwrap_or_default();
                let exists = find_vm_file(&self.manager, &archive, &file)
                    .map(|path| path.exists())
                    .unwrap_or_else(|| find_vm_resource(&self.manager, &archive, &file).is_some());
                return Ok(ethornell_vm::Value::Int(i32::from(exists)));
            }
            (0x80, 0x35) => {
                let file = pop_vm_string(stack).unwrap_or_else(|| "<unknown>".into());
                let archive = pop_vm_string(stack).unwrap_or_default();
                let size = find_vm_file(&self.manager, &archive, &file)
                    .and_then(|path| std::fs::metadata(path).ok())
                    .map(|meta| meta.len() as i32)
                    .or_else(|| {
                        find_vm_resource(&self.manager, &archive, &file)
                            .map(|entry| entry.unpacked_size.unwrap_or(entry.packed_size) as i32)
                    })
                    .unwrap_or(-1);
                return Ok(ethornell_vm::Value::Int(size));
            }
            _ => {}
        }
        self.fallback.call_sys(call)
    }
}

impl ethornell_vm::GraphApi for VmTraceApi {
    fn call_graph(
        &mut self,
        call: &mut ethornell_vm::NativeCallFrame,
    ) -> ethornell_vm::VmResult<ethornell_vm::Value> {
        self.fallback.call_graph(call)
    }
}

impl ethornell_vm::SoundApi for VmTraceApi {
    fn call_sound(
        &mut self,
        call: &mut ethornell_vm::NativeCallFrame,
    ) -> ethornell_vm::VmResult<ethornell_vm::Value> {
        self.fallback.call_sound(call)
    }
}

fn pop_vm_string(stack: &mut Vec<ethornell_vm::Value>) -> Option<String> {
    match stack.pop()? {
        ethornell_vm::Value::Str(text) => Some(text),
        ethornell_vm::Value::Ptr(ptr) => Some(format!("0x{ptr:08X}")),
        ethornell_vm::Value::Func { offset, .. } => Some(format!("0x{offset:08X}")),
        ethornell_vm::Value::Int(value) => Some(value.to_string()),
        ethornell_vm::Value::Program(_) | ethornell_vm::Value::None => None,
    }
}

fn find_vm_resource(
    manager: &ResourceManager,
    archive: &str,
    file: &str,
) -> Option<ethornell_archive::ResourceEntry> {
    let archive = archive.trim();
    if archive.is_empty() || archive == "0" {
        manager.find(file)
    } else {
        manager
            .find_in_archive(archive, file)
            .or_else(|| find_vm_wildcard_archive_resource(manager, archive, file))
    }
}

fn find_vm_wildcard_archive_resource(
    manager: &ResourceManager,
    archive: &str,
    file: &str,
) -> Option<ethornell_archive::ResourceEntry> {
    if !archive.contains("xxx") {
        return None;
    }
    let (prefix, suffix) = archive.split_once("xxx")?;
    manager.list().into_iter().find(|entry| {
        entry.entry_name.eq_ignore_ascii_case(file)
            && entry
                .archive_path
                .file_name()
                .and_then(|name| name.to_str())
                .map(|name| {
                    let name = name.to_ascii_lowercase();
                    name.starts_with(&prefix.to_ascii_lowercase())
                        && name.ends_with(&suffix.to_ascii_lowercase())
                })
                .unwrap_or(false)
    })
}

fn find_vm_file(manager: &ResourceManager, archive: &str, file: &str) -> Option<PathBuf> {
    let archive = archive.trim();
    if !archive.is_empty() && archive != "0" {
        return None;
    }
    let path = Path::new(file);
    if path.is_absolute() {
        Some(path.to_path_buf())
    } else {
        Some(manager.archives().root.join(path))
    }
}

fn run_text_test(text: &str) -> anyhow::Result<()> {
    ethornell_app::run_text_test(text.to_string()).map_err(Into::into)
}

fn run_view_image(file: &Path) -> anyhow::Result<()> {
    ethornell_app::view_image_file(file).map_err(Into::into)
}

fn run_view_resource_image(game: PathBuf, name: &str) -> anyhow::Result<()> {
    let game_root = GameRoot::new(game)?;
    ethornell_app::view_resource_image(game_root, name.to_string()).map_err(Into::into)
}

fn detect_entry_magic(name: &str, data: &[u8]) -> MagicKind {
    if name.to_ascii_lowercase().ends_with("._bp") {
        MagicKind::Bp
    } else {
        detect_magic(data)
    }
}

fn read_header(path: &Path) -> anyhow::Result<Vec<u8>> {
    use std::io::Read;
    let mut file = std::fs::File::open(path)?;
    let mut buf = vec![0u8; 64];
    let read = file.read(&mut buf)?;
    buf.truncate(read);
    Ok(buf)
}

fn run_decompile(
    file: Option<PathBuf>,
    game: Option<PathBuf>,
    archive: Option<String>,
    script: Option<String>,
    limit: Option<usize>,
    show_stack: bool,
) -> anyhow::Result<()> {
    let (label, bytes) = if let Some(file) = file {
        (
            file.display().to_string(),
            std::fs::read(&file).with_context(|| format!("read {}", file.display()))?,
        )
    } else {
        let game = game.context("decompile requires <file> or --game <dir> --script <name>")?;
        let script = script.context("decompile --game requires --script <name>")?;
        let manager = ResourceManager::open_game(game)?;
        let bytes = if let Some(archive) = archive {
            manager.read_decoded_from_archive(&archive, &script)?
        } else {
            manager.read_decoded(&script)?
        };
        (script.clone(), bytes)
    };
    if let Some(program) = parse_bcs(&bytes) {
        let text = decompile_bcs(&program, &DecompileOptions { limit, show_stack });
        println!("{text}");
        return Ok(());
    }
    let program = parse_bp_program(Some(label), &bytes);
    let text = decompile_bp(&program, &DecompileOptions { limit, show_stack });
    println!("{text}");
    Ok(())
}

fn run_call_catalog(
    game: PathBuf,
    json: bool,
    unknown_only: bool,
    all_registered: bool,
    generic_only: bool,
    limit_scripts: Option<usize>,
) -> anyhow::Result<()> {
    let manager = ResourceManager::open_game(&game)?;
    let mut sites = Vec::<CallSite>::new();
    let mut inferred_arg_counts = Vec::new();
    let mut script_count = 0usize;
    let limit = limit_scripts.unwrap_or(usize::MAX);

    for entry in manager
        .list()
        .into_iter()
        .filter(|entry| entry.entry_name.to_ascii_lowercase().ends_with("._bp"))
    {
        if script_count >= limit {
            break;
        }
        let bytes = manager.read_by_entry_decoded(&entry)?;
        if parse_bcs(&bytes).is_some() {
            continue;
        }
        let label = format!("{}:{}", entry.archive_path.display(), entry.entry_name);
        let program = parse_bp_program(Some(label), &bytes);
        sites.extend(scan_program_calls(&program));
        inferred_arg_counts.extend(infer_program_call_arg_counts(&program));
        script_count += 1;
    }

    for path in walk_files(&game)? {
        if script_count >= limit {
            break;
        }
        if !path
            .to_string_lossy()
            .to_ascii_lowercase()
            .ends_with("._bp")
        {
            continue;
        }
        let bytes = std::fs::read(&path)?;
        if parse_bcs(&bytes).is_some() {
            continue;
        }
        let program = parse_bp_program(Some(path.display().to_string()), &bytes);
        sites.extend(scan_program_calls(&program));
        inferred_arg_counts.extend(infer_program_call_arg_counts(&program));
        script_count += 1;
    }

    let mut summary = summarize_call_sites(sites);
    let inferred_by_key = summarize_inferred_arg_counts(inferred_arg_counts);
    for item in &mut summary {
        item.inferred_arg_counts = inferred_by_key.get(&item.key).cloned().unwrap_or_default();
    }
    if all_registered {
        let existing = summary.iter().map(|item| item.key).collect::<BTreeSet<_>>();
        for group in DISPATCH_GROUPS {
            for id in 0..=u8::MAX {
                let key = CallKey {
                    group,
                    id: u16::from(id),
                };
                if existing.contains(&key) {
                    continue;
                }
                if let Some(item) = registered_call_summary(key) {
                    summary.push(item);
                }
            }
        }
    }
    summary.sort_by(|left, right| {
        right
            .count
            .cmp(&left.count)
            .then(left.key.group.cmp(&right.key.group))
            .then(left.key.id.cmp(&right.key.id))
    });
    if unknown_only {
        summary.retain(|item| !item.known);
    }
    if generic_only {
        summary.retain(|item| item.generic);
    }

    if json {
        println!("{}", serde_json::to_string_pretty(&summary)?);
    } else {
        let used = summary.iter().filter(|item| item.used).count();
        let registered = summary.iter().filter(|item| item.registered).count();
        let generic = summary.iter().filter(|item| item.generic).count();
        println!(
            "scripts_scanned={script_count} calls={} used={used} registered={registered} generic={generic}",
            summary.len()
        );
        println!(
            "domain,group,id,name,count,argc,inferred_argc,known,registered,generic,used,scripts"
        );
        for item in summary {
            let domain = item
                .domain
                .map(|domain| domain.label())
                .unwrap_or("unknown");
            let name = item.name.unwrap_or("<unknown>");
            let argc = item
                .arg_count
                .map(|count| count.to_string())
                .unwrap_or_else(|| "?".to_string());
            let inferred_argc = item
                .inferred_arg_counts
                .iter()
                .take(4)
                .map(|value| format!("{}:{}", value.argc, value.count))
                .collect::<Vec<_>>()
                .join("|");
            let scripts = item.scripts.join(" | ");
            println!(
                "{domain},0x{:02X},0x{:02X},{name},{},{argc},{inferred_argc},{},{},{},{},{}",
                item.key.group,
                item.key.id,
                item.count,
                item.known,
                item.registered,
                item.generic,
                item.used,
                scripts
            );
        }
    }
    Ok(())
}

fn run_call_sites(
    game: PathBuf,
    archive: Option<&str>,
    script: Option<&str>,
    group: u8,
    id: u16,
    before: usize,
    after: usize,
    limit: Option<usize>,
    decompiled: bool,
) -> anyhow::Result<()> {
    let manager = ResourceManager::open_game(game)?;
    let mut programs = Vec::new();
    if let Some(script) = script {
        let bytes = if let Some(archive) = archive {
            manager.read_decoded_from_archive(archive, script)?
        } else {
            manager.read_decoded(script)?
        };
        if parse_bcs(&bytes).is_some() {
            bail!(
                "call-sites currently targets BP ._bp scripts; use disasm for BCS scenario files"
            );
        }
        programs.push((
            script.to_string(),
            parse_bp_program(Some(script.to_string()), &bytes),
        ));
    } else {
        let archive_filter = archive.map(|value| value.to_ascii_lowercase());
        for entry in manager.list() {
            if !entry.entry_name.to_ascii_lowercase().ends_with("._bp") {
                continue;
            }
            if archive_filter.as_ref().is_some_and(|filter| {
                entry
                    .archive_path
                    .file_name()
                    .and_then(|name| name.to_str())
                    .map(|name| name.to_ascii_lowercase() != *filter)
                    .unwrap_or(true)
            }) {
                continue;
            }
            let Ok(bytes) = manager.read_by_entry_decoded(&entry) else {
                continue;
            };
            if parse_bcs(&bytes).is_some() {
                continue;
            }
            let label = format!("{}:{}", entry.archive_path.display(), entry.entry_name);
            programs.push((label.clone(), parse_bp_program(Some(label), &bytes)));
        }
    }
    let mut shown = 0usize;
    'programs: for (script, program) in programs {
        let decompiled_lines = decompiled.then(|| decompiled_lines_by_offset(&program));
        for (index, instruction) in program.instructions.iter().enumerate() {
            let Some(key) = instruction_call_key(instruction) else {
                continue;
            };
            if key.group != group || key.id != id {
                continue;
            }
            if shown >= limit.unwrap_or(usize::MAX) {
                break 'programs;
            }
            shown += 1;
            let start = index.saturating_sub(before);
            let end = (index + after + 1).min(program.instructions.len());
            println!(
                "== {script} call #{shown} group=0x{group:02X} id=0x{id:02X} offset=0x{:08X} argc={} name={}",
                instruction.offset,
                key.arg_count()
                    .map(|count| count.to_string())
                    .unwrap_or_else(|| "?".to_string()),
                key.name().unwrap_or("<unknown>")
            );
            for (row, inst) in program.instructions[start..end].iter().enumerate() {
                let absolute = start + row;
                let marker = if absolute == index { "=>" } else { "  " };
                print_call_site_instruction(marker, inst);
            }
            if let Some(lines) = &decompiled_lines {
                println!("-- decompiled");
                for inst in &program.instructions[start..end] {
                    if let Some(line) = lines.get(&inst.offset) {
                        println!("{line}");
                    }
                }
            }
        }
    }
    println!("matches={shown}");
    Ok(())
}

fn decompiled_lines_by_offset(program: &ethornell_script::BpProgram) -> BTreeMap<u64, String> {
    decompile_bp(
        program,
        &DecompileOptions {
            limit: None,
            show_stack: true,
        },
    )
    .lines()
    .filter_map(|line| {
        let offset = line.get(0..8)?;
        if line.as_bytes().get(8) != Some(&b':') {
            return None;
        }
        let offset = u64::from_str_radix(offset, 16).ok()?;
        Some((offset, line.to_string()))
    })
    .collect()
}

fn print_call_site_instruction(marker: &str, inst: &BpInstruction) {
    let operands = inst
        .operands
        .iter()
        .map(|op| format!("{op:?}"))
        .collect::<Vec<_>>()
        .join(", ");
    let known = inst
        .known_call
        .map(|name| format!(" ; {name}"))
        .unwrap_or_default();
    println!(
        "{marker} {:08X} {:>4} {:<14} {:<28} raw={:02X?}{}",
        inst.offset, inst.opcode_hex(), inst.opcode.name(), operands, inst.raw, known
    );
}

fn run_disasm(
    file: Option<PathBuf>,
    game: Option<PathBuf>,
    json: bool,
    limit: Option<usize>,
    limit_files: usize,
) -> anyhow::Result<()> {
    if let Some(game) = game {
        let root = GameRoot::new(game)?;
        let mut files = Vec::new();
        for path in walk_files(root.path())? {
            if path
                .to_string_lossy()
                .to_ascii_lowercase()
                .ends_with("._bp")
            {
                files.push(path);
            }
        }
        for archive in archive_paths(&root)? {
            let index = read_archive_index(&archive)?;
            for entry in &index.entries {
                if entry.name.to_ascii_lowercase().ends_with("._bp") {
                    files.push(PathBuf::from(format!(
                        "{}:{}",
                        archive.display(),
                        entry.name
                    )));
                }
            }
        }
        println!("bp_files: {}", files.len());
        for path in files.into_iter().take(limit_files) {
            println!("== {}", path.display());
            if path.to_string_lossy().contains(".arc:") {
                let display = path.to_string_lossy();
                let (archive, name) = display
                    .split_once(".arc:")
                    .map(|(archive, name)| (format!("{archive}.arc"), name.to_string()))
                    .context("malformed archive-contained script path")?;
                let index = read_archive_index(Path::new(&archive))?;
                let entry = index
                    .entries
                    .iter()
                    .find(|entry| entry.name == name)
                    .context("script entry missing from archive index")?;
                let bytes = ethornell_archive::extract_entry(Path::new(&archive), entry)?;
                if let Some(bcs) = parse_bcs(&bytes) {
                    print_bcs_disasm(&bcs.commands, json, limit)?;
                } else {
                    print_disasm(&disassemble_bp(&bytes), json, limit)?;
                }
            } else {
                let bytes = std::fs::read(&path)?;
                if let Some(bcs) = parse_bcs(&bytes) {
                    print_bcs_disasm(&bcs.commands, json, limit)?;
                } else {
                    print_disasm(&disassemble_file(&path)?, json, limit)?;
                }
            }
        }
        return Ok(());
    }

    let file = file.context("disasm requires <file> or --game <dir>")?;
    let bytes = std::fs::read(&file)?;
    if let Some(bcs) = parse_bcs(&bytes) {
        print_bcs_disasm(&bcs.commands, json, limit)
    } else {
        print_disasm(&disassemble_file(&file)?, json, limit)
    }
}

fn print_bcs_disasm(
    commands: &[BcsCommand],
    json: bool,
    limit: Option<usize>,
) -> anyhow::Result<()> {
    let shown = commands.iter().take(limit.unwrap_or(usize::MAX));
    if json {
        let values: Vec<_> = shown.collect();
        println!("{}", serde_json::to_string_pretty(&values)?);
    } else {
        for command in shown {
            let name = command
                .name
                .map(str::to_string)
                .unwrap_or_else(|| format!("op_0x{:X}", command.opcode));
            println!(
                "{:08X} {:>6X} {:<18} {:?}",
                command.addr, command.opcode, name, command.args
            );
        }
    }
    Ok(())
}

fn print_disasm(
    instructions: &[BpInstruction],
    json: bool,
    limit: Option<usize>,
) -> anyhow::Result<()> {
    let shown = instructions.iter().take(limit.unwrap_or(usize::MAX));
    if json {
        let values: Vec<_> = shown.collect();
        println!("{}", serde_json::to_string_pretty(&values)?);
    } else {
        for inst in shown {
            let operands = inst
                .operands
                .iter()
                .map(|op| format!("{op:?}"))
                .collect::<Vec<_>>()
                .join(", ");
            let known = inst
                .known_call
                .map(|name| format!(" ; {name}"))
                .unwrap_or_default();
            let warning = inst
                .warning
                .as_ref()
                .map(|w| format!(" ; warning: {w}"))
                .unwrap_or_default();
            println!(
                "{:08X} {:>4} {:<14} {:<28} raw={:02X?}{}{}",
                inst.offset, inst.opcode_hex(), inst.opcode.name(), operands, inst.raw, known, warning
            );
        }
    }
    Ok(())
}

#[cfg(test)]
mod encoding_cli_tests {
    use super::*;
    use ethornell_app::TextEncoding;

    #[test]
    fn encoding_option_works_for_direct_launch_and_run() {
        for args in [
            vec!["ethornell", "--text-encoding", "auto", "game"],
            vec!["ethornell", "run", "--game", "game", "--text-encoding", "auto"],
            vec!["ethornell", "--text-encoding", "auto", "run", "--game", "game"],
        ] {
            assert_eq!(
                Cli::try_parse_from(args).unwrap().text_encoding,
                Some(TextEncoding::Auto),
            );
        }
        let cli = Cli::try_parse_from(["ethornell", "run", "--game", "game"]).unwrap();
        assert_eq!(cli.text_encoding, None); // environment, then Auto
        assert!(Cli::try_parse_from(["ethornell", "--text-encoding", "utf8", "game"]).is_err());
    }
}
