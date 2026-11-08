//! Optional network resource half of the native scoped policy. The existing
//! local descriptor policy continues to own user data and writer locking.
use super::*;
use ethornell_archive::ArchiveSource;
use game_network::{Directory,File};
use std::io::{Read,Seek,SeekFrom};
#[derive(Debug)]
pub(super) struct NetworkRoot { directory:Directory, pub mount:game_network::fs::Mount }
#[derive(Debug)]
struct Source(File);
impl Read for Source {fn read(&mut self,b:&mut[u8])->io::Result<usize>{self.0.read(b)}}
impl Seek for Source {fn seek(&mut self,s:SeekFrom)->io::Result<u64>{self.0.seek(s)}}
impl ArchiveSource for Source {fn byte_len(&mut self)->io::Result<u64>{Ok(self.0.len())}}
impl NetworkRoot {
    pub fn connect(url:&str)->io::Result<Self> {
        let directory=Directory::connect(url).map_err(|e|io::Error::other(format!("network resource connection: {e:#}")))?;
        Self::from_directory(directory)
    }
    pub fn from_directory(directory:Directory)->io::Result<Self> {
        let mount=game_network::fs::Mount::new(directory.clone()).map_err(io::Error::other)?;
        Ok(Self{directory,mount})
    }
    pub fn metadata(&self,name:&str)->Option<PolicyMetadata>{
        let path=relative(name)?;
        let m=self.directory.metadata(&path.to_string_lossy().replace('\\',"/")).ok()?;
        Some(PolicyMetadata{is_dir:m.directory,len:m.len,readonly:true})
    }
    pub fn read(&self,name:&str,limit:u64)->io::Result<Vec<u8>> {
        let path=relative(name).ok_or_else(||io::Error::from(io::ErrorKind::PermissionDenied))?;
        let file=self.directory.open(&path.to_string_lossy().replace('\\',"/"))?;
        if file.len()>limit {return Err(io::Error::other("network resource file budget exceeded"));}
        let mut out=Vec::new();file.take(limit+1).read_to_end(&mut out)?;
        if out.len() as u64>limit {return Err(io::Error::other("network resource file budget exceeded"));}Ok(out)
    }
    fn walk(&self,dir:&str,depth:usize,total:&mut usize,recursive:bool,visit:&mut dyn FnMut(&str,game_network::Metadata)->io::Result<()>)->io::Result<()> {
        if depth>64 {return Err(io::Error::other("network resource directory depth exceeded"));}
        for name in self.directory.list(dir)? {
            *total+=1;if *total>262144 {return Err(io::Error::other("network resource entry budget exceeded"));}
            let path=if dir.is_empty(){name}else{format!("{dir}/{name}")};
            let meta=self.directory.metadata(&path)?;visit(&path,meta)?;
            if recursive && meta.directory {self.walk(&path,depth+1,total,true,visit)?;}
        }Ok(())
    }
    pub fn validate(&self)->io::Result<()> {self.walk("",0,&mut 0,true,&mut |_,_|Ok(()))}
    pub fn manager(&self,root:&Path)->ethornell_core::Result<ethornell_archive::ResourceManager> {
        let mut files:Vec<(PathBuf,Box<dyn ArchiveSource>)>=Vec::new();
        self.walk("",0,&mut 0,true,&mut |path,meta| {
            if meta.directory{return Ok(());}
            let mut source=self.directory.open(path)?;
            let mut header=[0;12];let n=source.read(&mut header)?;
            if ethornell_archive::detect_archive_format_from_bytes(&header[..n])==ethornell_archive::ArchiveFormat::Unknown{return Ok(());}
            if files.len()>=512 {return Err(io::Error::other("network archive descriptor budget exceeded"));}
            source.seek(SeekFrom::Start(0))?;
            files.push((root.join(path),Box::new(Source(source))));Ok(())
        })?;
        files.sort_by(|a,b|a.0.cmp(&b.0));
        ethornell_archive::ResourceManager::from_authorized_sources_with_limits(root.to_owned(),files,ethornell_archive::ResourceBudget::android())
    }
    pub fn enumerate(&self,pattern:&str,recursive:bool,max:usize,dirs:bool)->io::Result<Vec<String>> {
        let path=relative(pattern).ok_or_else(||io::Error::from(io::ErrorKind::PermissionDenied))?;
        let parent=path.parent().unwrap_or(Path::new("")).to_string_lossy().replace('\\',"/");
        let wildcard=path.file_name().and_then(|s|s.to_str()).unwrap_or("*");
        let limit=if max==0{10000}else{max.min(10000)};
        let mut out=Vec::new();
        self.walk(&parent,0,&mut 0,recursive,&mut |name,meta|{
            let path=Path::new(name);let leaf=path.file_name().unwrap().to_string_lossy();
            if out.len()<limit && meta.directory==dirs && !leaf.starts_with(".ethornell-") && super::super::windows_wildcard_matches(wildcard,&leaf){
                out.push(path.strip_prefix(&parent).unwrap_or(path).to_string_lossy().trim_start_matches('/').replace('/',"\\"));
            }Ok(())
        })?;Ok(out)
    }
}
