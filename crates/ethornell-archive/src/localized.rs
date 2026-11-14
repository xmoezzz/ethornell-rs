//! Session-local views of publisher language archives. The physical archive
//! names remain available to scripts; canonical names search the selected
//! language first and then that family's untagged shared archive.
use super::*;

#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub enum ArchiveLanguage {
    Japanese,
    English,
    SimplifiedChinese,
    TraditionalChinese,
    Korean,
}

impl ArchiveLanguage {
    pub fn from_tag(tag: &str) -> Option<Self> {
        match tag.to_ascii_lowercase().as_str() {
            "ja" | "jp" | "jpn" => Some(Self::Japanese),
            "en" | "eng" => Some(Self::English),
            "cn" | "chs" | "sc" | "zh-cn" | "zh-hans" => Some(Self::SimplifiedChinese),
            "tc" | "cht" | "tw" | "zh-tw" | "zh-hant" => Some(Self::TraditionalChinese),
            "ko" | "kr" | "kor" => Some(Self::Korean),
            _ => None,
        }
    }
}

fn tagged(name: &str) -> Option<(String, &str, ArchiveLanguage)> {
    let stem = name.strip_suffix(".arc")?;
    let (family, tag) = stem.rsplit_once('.')?;
    if family.is_empty() {
        return None;
    }
    Some((
        format!("{family}.arc"),
        tag,
        ArchiveLanguage::from_tag(tag)?,
    ))
}

#[derive(Debug)]
pub(super) struct ArchiveView {
    language: ArchiveLanguage,
    aliases: HashMap<String, Vec<usize>>,
    visible: Vec<Vec<bool>>,
    rank: Vec<usize>,
}

impl ResourceManager {
    /// Select one coherent publisher language without renaming any files.
    /// An ordinary untagged bootstrap keeps the historical view. Otherwise
    /// English is the default when available, followed by a stable language
    /// order. A requested language must actually provide a bootstrap.
    pub fn for_native_bootstrap(&self, preferred: Option<ArchiveLanguage>) -> Result<Self> {
        let has_bootstrap = |index: usize| {
            self.inner.set.archives[index]
                .entries
                .iter()
                .any(|entry| normalize_resource_name(&entry.name) == "ipl._bp")
        };
        if preferred.is_none()
            && self
                .inner
                .archive_by_name
                .get("system.arc")
                .is_some_and(|&index| has_bootstrap(index))
        {
            return Ok(Self {
                inner: self.inner.clone(),
                view: None,
            });
        }
        let mut candidates = self
            .inner
            .archive_by_name
            .iter()
            .filter_map(|(name, &index)| {
                let (family, tag, language) = tagged(name)?;
                (family == "system.arc" && has_bootstrap(index)).then_some((
                    language,
                    tag.to_owned(),
                    index,
                ))
            })
            .collect::<Vec<_>>();
        candidates.sort_by_key(|(language, tag, index)| {
            (
                *language != ArchiveLanguage::English,
                *language,
                tag.clone(),
                *index,
            )
        });
        let Some((language, tag, _)) = candidates
            .iter()
            .find(|(language, _, _)| preferred.is_none_or(|wanted| wanted == *language))
        else {
            return Err(EthornellError::Parse(match preferred {
                Some(language) => format!("no native bootstrap for archive language {language:?}"),
                None => {
                    "native bootstrap ipl._bp is absent from system.arc and its language archives"
                        .into()
                }
            }));
        };
        let mut families: HashMap<String, Vec<(String, usize)>> = HashMap::new();
        for (name, &index) in &self.inner.archive_by_name {
            if let Some((family, member_tag, member_language)) = tagged(name)
                && member_language == *language
            {
                families
                    .entry(family)
                    .or_default()
                    .push((member_tag.to_owned(), index));
            }
        }
        let mut aliases = HashMap::new();
        for (family, mut members) in families {
            members
                .sort_by_key(|(member_tag, index)| (member_tag != tag, member_tag.clone(), *index));
            let mut indices = vec![members[0].1];
            if let Some(&base) = self.inner.archive_by_name.get(&family) {
                indices.push(base);
            }
            aliases.insert(family, indices);
        }
        let mut visible = self
            .inner
            .set
            .archives
            .iter()
            .enumerate()
            .map(|(index, archive)| {
                let name = archive_name_key(&archive.path.to_string_lossy());
                let enabled = match tagged(&name) {
                    Some((family, _, _)) => aliases
                        .get(&family)
                        .is_some_and(|indices| indices[0] == index),
                    None => true,
                };
                vec![enabled; archive.entries.len()]
            })
            .collect::<Vec<_>>();
        for indices in aliases.values() {
            if let [local, base] = indices.as_slice() {
                let translated = self.inner.set.archives[*local]
                    .entries
                    .iter()
                    .map(|entry| normalize_resource_name(&entry.name))
                    .collect::<std::collections::HashSet<_>>();
                for (entry, enabled) in self.inner.set.archives[*base]
                    .entries
                    .iter()
                    .zip(&mut visible[*base])
                {
                    if translated.contains(&normalize_resource_name(&entry.name)) {
                        *enabled = false;
                    }
                }
            }
        }
        let mut order = (0..self.inner.set.archives.len()).collect::<Vec<_>>();
        order.sort_by_key(|&index| {
            let name = archive_name_key(&self.inner.set.archives[index].path.to_string_lossy());
            (tagged(&name).is_none(), index)
        });
        let mut rank = vec![0; order.len()];
        for (position, index) in order.into_iter().enumerate() {
            rank[index] = position;
        }
        Ok(Self {
            inner: self.inner.clone(),
            view: Some(Arc::new(ArchiveView {
                language: *language,
                aliases,
                visible,
                rank,
            })),
        })
    }

    pub fn archive_language(&self) -> Option<ArchiveLanguage> {
        self.view.as_ref().map(|view| view.language)
    }

    pub(super) fn in_archive_family<T>(
        &self,
        name: &str,
        mut find: impl FnMut(usize) -> Option<T>,
    ) -> Option<T> {
        let key = archive_name_key(name);
        if let Some(view) = &self.view
            && let Some(indices) = view.aliases.get(&key)
        {
            return indices.iter().copied().find_map(find);
        }
        self.inner
            .archive_by_name
            .get(&key)
            .and_then(|&index| find(index))
    }

    pub(super) fn entry_visible(&self, archive: usize, entry: usize) -> bool {
        self.view
            .as_ref()
            .is_none_or(|view| view.visible[archive][entry])
    }

    pub(super) fn preferred_locator(&self, locators: &[(usize, usize)]) -> Option<(usize, usize)> {
        locators
            .iter()
            .copied()
            .filter(|&(archive, entry)| self.entry_visible(archive, entry))
            .min_by_key(|&(archive, _)| {
                self.view
                    .as_ref()
                    .map_or(archive, |view| view.rank[archive])
            })
    }
}
