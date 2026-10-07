#[cfg(not(any(target_os = "macos", target_os = "windows")))]
use std::io::Write;
use std::path::{Path, PathBuf};
use std::process::Command;
#[cfg(not(any(target_os = "macos", target_os = "windows")))]
use std::process::Stdio;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) enum PathDialogMode {
    OpenFile,
    SaveFile,
    Folder,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(crate) struct PathDialogFilter {
    pub label: String,
    pub extension: String,
}

fn clean_output(bytes: &[u8]) -> Option<String> {
    let value = String::from_utf8_lossy(bytes).trim().to_string();
    (!value.is_empty()).then_some(value)
}

fn initial_selection(initial_directory: &str, default_name: &str) -> String {
    if default_name.trim().is_empty() {
        return initial_directory.to_string();
    }
    if initial_directory.trim().is_empty() {
        return default_name.to_string();
    }
    Path::new(initial_directory)
        .join(default_name)
        .to_string_lossy()
        .into_owned()
}

#[cfg(target_os = "macos")]
fn choose_path_platform(
    mode: PathDialogMode,
    title: &str,
    initial_directory: &str,
    default_name: &str,
    _filters: &[PathDialogFilter],
) -> Option<String> {
    let script = match mode {
        PathDialogMode::OpenFile => {
            r#"on run argv
set promptText to item 1 of argv
set initialPath to item 2 of argv
if initialPath is "" then
    set chosenItem to choose file with prompt promptText
else
    set chosenItem to choose file with prompt promptText default location POSIX file initialPath
end if
return POSIX path of chosenItem
end run"#
        }
        PathDialogMode::SaveFile => {
            r#"on run argv
set promptText to item 1 of argv
set initialPath to item 2 of argv
set defaultName to item 3 of argv
if initialPath is "" then
    set chosenItem to choose file name with prompt promptText default name defaultName
else
    set chosenItem to choose file name with prompt promptText default location POSIX file initialPath default name defaultName
end if
return POSIX path of chosenItem
end run"#
        }
        PathDialogMode::Folder => {
            r#"on run argv
set promptText to item 1 of argv
set initialPath to item 2 of argv
if initialPath is "" then
    set chosenItem to choose folder with prompt promptText
else
    set chosenItem to choose folder with prompt promptText default location POSIX file initialPath
end if
return POSIX path of chosenItem
end run"#
        }
    };
    let mut command = Command::new("/usr/bin/osascript");
    command
        .arg("-e")
        .arg(script)
        .arg("--")
        .arg(title)
        .arg(initial_directory);
    if mode == PathDialogMode::SaveFile {
        command.arg(default_name);
    }
    let output = command.output().ok()?;
    output
        .status
        .success()
        .then(|| clean_output(&output.stdout))
        .flatten()
}

#[cfg(target_os = "windows")]
fn choose_path_platform(
    mode: PathDialogMode,
    title: &str,
    initial_directory: &str,
    default_name: &str,
    filters: &[PathDialogFilter],
) -> Option<String> {
    let filter = if filters.is_empty() {
        "All files (*.*)|*.*".to_string()
    } else {
        filters
            .iter()
            .flat_map(|entry| {
                let extension = entry
                    .extension
                    .trim()
                    .trim_start_matches("*.")
                    .trim_start_matches('.');
                let pattern = if extension.is_empty() {
                    "*.*".to_string()
                } else {
                    format!("*.{extension}")
                };
                let label = if entry.label.trim().is_empty() {
                    pattern.clone()
                } else {
                    entry.label.clone()
                };
                [format!("{label} ({pattern})"), pattern]
            })
            .chain(["All files (*.*)".to_string(), "*.*".to_string()])
            .collect::<Vec<_>>()
            .join("|")
    };
    let script = match mode {
        PathDialogMode::OpenFile => {
            r#"
Add-Type -AssemblyName System.Windows.Forms
$d = New-Object System.Windows.Forms.OpenFileDialog
$d.Title = $env:BGI_DIALOG_TITLE
$d.InitialDirectory = $env:BGI_DIALOG_INITIAL
$d.FileName = $env:BGI_DIALOG_DEFAULT
$d.Filter = $env:BGI_DIALOG_FILTER
if ($d.ShowDialog() -eq [System.Windows.Forms.DialogResult]::OK) {
  [Console]::OutputEncoding = [System.Text.Encoding]::UTF8
  Write-Output $d.FileName
  exit 0
}
exit 1
"#
        }
        PathDialogMode::SaveFile => {
            r#"
Add-Type -AssemblyName System.Windows.Forms
$d = New-Object System.Windows.Forms.SaveFileDialog
$d.Title = $env:BGI_DIALOG_TITLE
$d.InitialDirectory = $env:BGI_DIALOG_INITIAL
$d.FileName = $env:BGI_DIALOG_DEFAULT
$d.Filter = $env:BGI_DIALOG_FILTER
if ($d.ShowDialog() -eq [System.Windows.Forms.DialogResult]::OK) {
  [Console]::OutputEncoding = [System.Text.Encoding]::UTF8
  Write-Output $d.FileName
  exit 0
}
exit 1
"#
        }
        PathDialogMode::Folder => {
            r#"
Add-Type -AssemblyName System.Windows.Forms
$d = New-Object System.Windows.Forms.FolderBrowserDialog
$d.Description = $env:BGI_DIALOG_TITLE
$d.SelectedPath = $env:BGI_DIALOG_INITIAL
if ($d.ShowDialog() -eq [System.Windows.Forms.DialogResult]::OK) {
  [Console]::OutputEncoding = [System.Text.Encoding]::UTF8
  Write-Output $d.SelectedPath
  exit 0
}
exit 1
"#
        }
    };
    for executable in ["powershell.exe", "pwsh.exe"] {
        let output = Command::new(executable)
            .args(["-NoProfile", "-NonInteractive", "-Command", script])
            .env("BGI_DIALOG_TITLE", title)
            .env("BGI_DIALOG_INITIAL", initial_directory)
            .env("BGI_DIALOG_DEFAULT", default_name)
            .env("BGI_DIALOG_FILTER", &filter)
            .output();
        if let Ok(output) = output {
            if output.status.success() {
                return clean_output(&output.stdout);
            }
            if output.status.code().is_some() {
                return None;
            }
        }
    }
    None
}

#[cfg(not(any(target_os = "macos", target_os = "windows")))]
fn choose_path_platform(
    mode: PathDialogMode,
    title: &str,
    initial_directory: &str,
    default_name: &str,
    filters: &[PathDialogFilter],
) -> Option<String> {
    let selection = initial_selection(initial_directory, default_name);
    let mut zenity = Command::new("zenity");
    zenity
        .arg("--file-selection")
        .arg(format!("--title={title}"));
    match mode {
        PathDialogMode::OpenFile => {}
        PathDialogMode::SaveFile => {
            zenity.arg("--save").arg("--confirm-overwrite");
        }
        PathDialogMode::Folder => {
            zenity.arg("--directory");
        }
    }
    if !selection.is_empty() {
        zenity.arg(format!("--filename={selection}"));
    }
    for entry in filters {
        let extension = entry
            .extension
            .trim()
            .trim_start_matches("*.")
            .trim_start_matches('.');
        if !extension.is_empty() {
            let label = if entry.label.trim().is_empty() {
                format!("*.{extension}")
            } else {
                entry.label.clone()
            };
            zenity.arg(format!("--file-filter={label} | *.{extension}"));
        }
    }
    if let Ok(output) = zenity.output() {
        if output.status.success() {
            return clean_output(&output.stdout);
        }
        if output.status.code().is_some() {
            return None;
        }
    }

    let mut kdialog = Command::new("kdialog");
    match mode {
        PathDialogMode::OpenFile => {
            kdialog.arg("--getopenfilename");
        }
        PathDialogMode::SaveFile => {
            kdialog.arg("--getsavefilename");
        }
        PathDialogMode::Folder => {
            kdialog.arg("--getexistingdirectory");
        }
    }
    kdialog.arg(if selection.is_empty() {
        "."
    } else {
        selection.as_str()
    });
    if mode != PathDialogMode::Folder {
        let filter = filters
            .iter()
            .filter_map(|entry| {
                let extension = entry
                    .extension
                    .trim()
                    .trim_start_matches("*.")
                    .trim_start_matches('.');
                (!extension.is_empty()).then(|| format!("*.{extension}|{}", entry.label))
            })
            .collect::<Vec<_>>()
            .join("\n");
        if !filter.is_empty() {
            kdialog.arg(filter);
        }
    }
    kdialog.arg("--title").arg(title);
    let output = kdialog.output().ok()?;
    output
        .status
        .success()
        .then(|| clean_output(&output.stdout))
        .flatten()
}

pub(crate) fn choose_path(
    mode: PathDialogMode,
    title: &str,
    initial_directory: &str,
    default_name: &str,
    filters: &[PathDialogFilter],
) -> Option<String> {
    choose_path_platform(mode, title, initial_directory, default_name, filters).map(|path| {
        let path = PathBuf::from(path);
        path.to_string_lossy().into_owned()
    })
}

#[cfg(target_os = "macos")]
fn show_text_platform(title: &str, text: &str) -> bool {
    let script = r#"on run argv
set dialogTitle to item 1 of argv
set dialogText to item 2 of argv
display dialog dialogText with title dialogTitle buttons {"Cancel", "OK"} default button "OK" cancel button "Cancel"
return "OK"
end run"#;
    Command::new("/usr/bin/osascript")
        .arg("-e")
        .arg(script)
        .arg("--")
        .arg(title)
        .arg(text.chars().take(8_000).collect::<String>())
        .status()
        .map(|status| status.success())
        .unwrap_or(false)
}

#[cfg(target_os = "windows")]
fn show_text_platform(title: &str, text: &str) -> bool {
    let script = r#"
Add-Type -AssemblyName System.Windows.Forms
$result = [System.Windows.Forms.MessageBox]::Show($env:BGI_DIALOG_TEXT, $env:BGI_DIALOG_TITLE, [System.Windows.Forms.MessageBoxButtons]::OKCancel, [System.Windows.Forms.MessageBoxIcon]::Information)
if ($result -eq [System.Windows.Forms.DialogResult]::OK) { exit 0 }
exit 1
"#;
    for executable in ["powershell.exe", "pwsh.exe"] {
        if let Ok(status) = Command::new(executable)
            .args(["-NoProfile", "-NonInteractive", "-Command", script])
            .env("BGI_DIALOG_TITLE", title)
            .env(
                "BGI_DIALOG_TEXT",
                text.chars().take(30_000).collect::<String>(),
            )
            .status()
        {
            return status.success();
        }
    }
    false
}

#[cfg(not(any(target_os = "macos", target_os = "windows")))]
fn show_text_platform(title: &str, text: &str) -> bool {
    if let Ok(mut child) = Command::new("zenity")
        .args(["--text-info", "--width=700", "--height=500"])
        .arg(format!("--title={title}"))
        .stdin(Stdio::piped())
        .spawn()
    {
        if let Some(mut stdin) = child.stdin.take() {
            let _ = stdin.write_all(text.as_bytes());
        }
        return child.wait().map(|status| status.success()).unwrap_or(false);
    }
    Command::new("kdialog")
        .args(["--textbox", "/dev/stdin", "700", "500", "--title", title])
        .stdin(Stdio::piped())
        .spawn()
        .and_then(|mut child| {
            if let Some(mut stdin) = child.stdin.take() {
                stdin.write_all(text.as_bytes())?;
            }
            child.wait()
        })
        .map(|status| status.success())
        .unwrap_or(false)
}

pub(crate) fn show_text(title: &str, text: &str) -> bool {
    show_text_platform(title, text)
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) enum MessageDialogKind {
    Information,
    YesNo,
    OkCancel,
    RetryCancel,
}

#[cfg(target_os = "macos")]
fn show_message_platform(
    kind: MessageDialogKind,
    title: &str,
    text: &str,
    default_first: bool,
) -> Option<bool> {
    let (first, second) = match kind {
        MessageDialogKind::Information => ("OK", ""),
        MessageDialogKind::YesNo => ("Yes", "No"),
        MessageDialogKind::OkCancel => ("OK", "Cancel"),
        MessageDialogKind::RetryCancel => ("Retry", "Quit"),
    };
    let default = if default_first || second.is_empty() { first } else { second };
    let buttons = if second.is_empty() {
        format!(r#"buttons {{"{first}"}} default button "{first}""#)
    } else {
        format!(
            r#"buttons {{"{second}", "{first}"}} default button "{default}" cancel button "{second}""#
        )
    };
    let activate = "tell application \"System Events\" to activate\n";
    let script = format!(
        "on run argv\nset dialogTitle to item 1 of argv\nset dialogText to item 2 of argv\n{activate}display dialog dialogText with title dialogTitle {buttons}\nreturn \"1\"\nend run"
    );
    let status = Command::new("/usr/bin/osascript")
        .arg("-e")
        .arg(script)
        .arg("--")
        .arg(title)
        .arg(text.chars().take(8_000).collect::<String>())
        .status()
        .ok()?;
    Some(status.success())
}

#[cfg(target_os = "windows")]
fn show_message_platform(
    kind: MessageDialogKind,
    title: &str,
    text: &str,
    default_first: bool,
) -> Option<bool> {
    let icon = match kind {
        MessageDialogKind::YesNo => "Question",
        _ => "Information",
    };
    let buttons = match kind {
        MessageDialogKind::Information => "OK",
        MessageDialogKind::YesNo => "YesNo",
        MessageDialogKind::OkCancel => "OKCancel",
        MessageDialogKind::RetryCancel => "RetryCancel",
    };
    let script = r#"
Add-Type -AssemblyName System.Windows.Forms
$buttons = [System.Enum]::Parse([System.Windows.Forms.MessageBoxButtons], $env:BGI_DIALOG_BUTTONS)
$icon = [System.Enum]::Parse([System.Windows.Forms.MessageBoxIcon], $env:BGI_DIALOG_ICON)
$default = [System.Enum]::Parse([System.Windows.Forms.MessageBoxDefaultButton], $env:BGI_DIALOG_DEFAULT)
$result = [System.Windows.Forms.MessageBox]::Show($env:BGI_DIALOG_TEXT, $env:BGI_DIALOG_TITLE, $buttons, $icon, $default)
if ($result -eq [System.Windows.Forms.DialogResult]::OK -or $result -eq [System.Windows.Forms.DialogResult]::Yes -or $result -eq [System.Windows.Forms.DialogResult]::Retry) { exit 0 }
exit 1
"#;
    for executable in ["powershell.exe", "pwsh.exe"] {
        let status = Command::new(executable)
            .args(["-NoProfile", "-NonInteractive", "-Command", script])
            .env("BGI_DIALOG_TITLE", title)
            .env(
                "BGI_DIALOG_TEXT",
                text.chars().take(30_000).collect::<String>(),
            )
            .env("BGI_DIALOG_BUTTONS", buttons)
            .env("BGI_DIALOG_ICON", icon)
            .env(
                "BGI_DIALOG_DEFAULT",
                if default_first { "Button1" } else { "Button2" },
            )
            .status();
        if let Ok(status) = status {
            return Some(status.success());
        }
    }
    None
}

#[cfg(not(any(target_os = "macos", target_os = "windows")))]
fn show_message_platform(
    kind: MessageDialogKind,
    title: &str,
    text: &str,
    default_first: bool,
) -> Option<bool> {
    let mut zenity = Command::new("zenity");
    if !default_first && kind != MessageDialogKind::Information {
        zenity.arg("--default-cancel");
    }
    match kind {
        MessageDialogKind::Information => {
            zenity.arg("--info");
        }
        MessageDialogKind::YesNo => {
            zenity
                .arg("--question")
                .arg("--ok-label=Yes")
                .arg("--cancel-label=No");
        }
        MessageDialogKind::OkCancel => {
            zenity
                .arg("--question")
                .arg("--ok-label=OK")
                .arg("--cancel-label=Cancel");
        }
        MessageDialogKind::RetryCancel => {
            zenity
                .arg("--question")
                .arg("--ok-label=Retry")
                .arg("--cancel-label=Quit");
        }
    }
    zenity.arg(format!("--title={title}")).arg(format!(
        "--text={}",
        text.chars().take(30_000).collect::<String>()
    ));
    if let Ok(status) = zenity.status() {
        return Some(status.success());
    }

    let mut kdialog = Command::new("kdialog");
    match kind {
        MessageDialogKind::Information => {
            kdialog.arg("--msgbox");
        }
        MessageDialogKind::YesNo | MessageDialogKind::OkCancel | MessageDialogKind::RetryCancel => {
            kdialog.arg("--yesno");
        }
    }
    let status = kdialog.arg(text).arg("--title").arg(title).status().ok()?;
    Some(status.success())
}

/// MessageBoxA as used by sub_46BC80. `default_first` keeps the first
/// button (OK / Yes) as the default; otherwise MB_DEFBUTTON2 makes the second
/// (Cancel / No) the default.
pub(crate) fn show_message(
    kind: MessageDialogKind,
    title: &str,
    text: &str,
    default_first: bool,
) -> Option<bool> {
    show_message_platform(kind, title, text, default_first)
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) enum TextInputConstraint {
    Any,
    SignedDecimal,
    Alphanumeric,
}

fn truncate_shift_jis(text: &str, max_bytes: usize) -> String {
    if max_bytes == 0 {
        return String::new();
    }
    let mut output = String::new();
    for ch in text.chars() {
        let mut candidate = output.clone();
        candidate.push(ch);
        if encoding_rs::SHIFT_JIS.encode(&candidate).0.len() > max_bytes {
            break;
        }
        output.push(ch);
    }
    output
}

fn constrain_text(text: &str, max_bytes: usize, constraint: TextInputConstraint) -> String {
    let filtered = match constraint {
        TextInputConstraint::Any => text.to_string(),
        TextInputConstraint::SignedDecimal => {
            let mut output = String::new();
            for (index, ch) in text.chars().enumerate() {
                if ch.is_ascii_digit() || (index == 0 && ch == '-') {
                    output.push(ch);
                }
            }
            output
        }
        TextInputConstraint::Alphanumeric => text
            .chars()
            .filter(|ch| ch.is_ascii_alphanumeric())
            .collect(),
    };
    truncate_shift_jis(&filtered, max_bytes)
}

#[cfg(target_os = "macos")]
fn prompt_text_platform(title: &str, prompt: &str, initial: &str) -> Option<String> {
    let script = r#"on run argv
set dialogTitle to item 1 of argv
set promptText to item 2 of argv
set initialText to item 3 of argv
set answer to display dialog promptText with title dialogTitle default answer initialText buttons {"Cancel", "OK"} default button "OK" cancel button "Cancel"
return text returned of answer
end run"#;
    let output = Command::new("/usr/bin/osascript")
        .arg("-e")
        .arg(script)
        .arg("--")
        .arg(title)
        .arg(prompt)
        .arg(initial)
        .output()
        .ok()?;
    output
        .status
        .success()
        .then(|| clean_output_preserve_empty(&output.stdout))
        .flatten()
}

#[cfg(target_os = "windows")]
fn prompt_text_platform(title: &str, prompt: &str, initial: &str) -> Option<String> {
    let script = r#"
Add-Type -AssemblyName System.Windows.Forms
Add-Type -AssemblyName System.Drawing
$form = New-Object System.Windows.Forms.Form
$form.Text = $env:BGI_DIALOG_TITLE
$form.StartPosition = 'CenterScreen'
$form.Width = 520
$form.Height = 190
$form.FormBorderStyle = 'FixedDialog'
$form.MaximizeBox = $false
$form.MinimizeBox = $false
$label = New-Object System.Windows.Forms.Label
$label.Left = 12
$label.Top = 12
$label.Width = 480
$label.Height = 42
$label.Text = $env:BGI_DIALOG_PROMPT
$text = New-Object System.Windows.Forms.TextBox
$text.Left = 12
$text.Top = 58
$text.Width = 480
$text.Text = $env:BGI_DIALOG_INITIAL
$ok = New-Object System.Windows.Forms.Button
$ok.Text = 'OK'
$ok.Left = 332
$ok.Top = 96
$ok.Width = 75
$ok.DialogResult = [System.Windows.Forms.DialogResult]::OK
$cancel = New-Object System.Windows.Forms.Button
$cancel.Text = 'Cancel'
$cancel.Left = 417
$cancel.Top = 96
$cancel.Width = 75
$cancel.DialogResult = [System.Windows.Forms.DialogResult]::Cancel
$form.Controls.AddRange(@($label, $text, $ok, $cancel))
$form.AcceptButton = $ok
$form.CancelButton = $cancel
$form.Add_Shown({ $text.Select(); $text.SelectionStart = $text.Text.Length })
if ($form.ShowDialog() -eq [System.Windows.Forms.DialogResult]::OK) {
  [Console]::OutputEncoding = [System.Text.Encoding]::UTF8
  [Console]::Write($text.Text)
  exit 0
}
exit 1
"#;
    for executable in ["powershell.exe", "pwsh.exe"] {
        let output = Command::new(executable)
            .args(["-NoProfile", "-STA", "-Command", script])
            .env("BGI_DIALOG_TITLE", title)
            .env("BGI_DIALOG_PROMPT", prompt)
            .env("BGI_DIALOG_INITIAL", initial)
            .output();
        if let Ok(output) = output {
            if output.status.success() {
                return Some(String::from_utf8_lossy(&output.stdout).to_string());
            }
            if output.status.code().is_some() {
                return None;
            }
        }
    }
    None
}

#[cfg(not(any(target_os = "macos", target_os = "windows")))]
fn prompt_text_platform(title: &str, prompt: &str, initial: &str) -> Option<String> {
    let output = Command::new("zenity")
        .arg("--entry")
        .arg(format!("--title={title}"))
        .arg(format!("--text={prompt}"))
        .arg(format!("--entry-text={initial}"))
        .output();
    if let Ok(output) = output {
        if output.status.success() {
            return Some(
                String::from_utf8_lossy(&output.stdout)
                    .trim_end_matches(['\r', '\n'])
                    .to_string(),
            );
        }
        if output.status.code().is_some() {
            return None;
        }
    }
    let output = Command::new("kdialog")
        .arg("--inputbox")
        .arg(prompt)
        .arg(initial)
        .arg("--title")
        .arg(title)
        .output()
        .ok()?;
    output.status.success().then(|| {
        String::from_utf8_lossy(&output.stdout)
            .trim_end_matches(['\r', '\n'])
            .to_string()
    })
}

fn clean_output_preserve_empty(bytes: &[u8]) -> Option<String> {
    Some(
        String::from_utf8_lossy(bytes)
            .trim_end_matches(['\r', '\n'])
            .to_string(),
    )
}

pub(crate) fn prompt_text(
    title: &str,
    prompt: &str,
    initial: &str,
    max_bytes: usize,
    constraint: TextInputConstraint,
) -> Option<String> {
    let value = prompt_text_platform(title, prompt, initial)?;
    Some(constrain_text(&value, max_bytes, constraint))
}

pub(crate) fn prompt_two_fields(
    title: &str,
    first_label: &str,
    first_initial: &str,
    first_max_bytes: usize,
    first_constraint: TextInputConstraint,
    second_label: &str,
    second_initial: &str,
    second_max_bytes: usize,
    second_constraint: TextInputConstraint,
) -> Option<[String; 2]> {
    let first = prompt_text(
        title,
        first_label,
        first_initial,
        first_max_bytes,
        first_constraint,
    )?;
    let second = prompt_text(
        title,
        second_label,
        second_initial,
        second_max_bytes,
        second_constraint,
    )?;
    Some([first, second])
}

pub(crate) fn prompt_segmented(
    title: &str,
    prompt: &str,
    segment_count: usize,
    max_bytes_per_segment: usize,
    constraint: TextInputConstraint,
) -> Option<String> {
    let mut segments = Vec::with_capacity(segment_count);
    for index in 0..segment_count {
        let label = if prompt.is_empty() {
            format!("Part {}", index + 1)
        } else {
            format!("{prompt} ({}/{segment_count})", index + 1)
        };
        segments.push(prompt_text(
            title,
            &label,
            "",
            max_bytes_per_segment,
            constraint,
        )?);
    }
    Some(segments.join("-"))
}

#[cfg(target_os = "macos")]
fn choose_item_platform(title: &str, prompt: &str, options: &[String]) -> Option<String> {
    if options.is_empty() {
        return Some(String::new());
    }
    let script = r#"on run argv
set dialogTitle to item 1 of argv
set promptText to item 2 of argv
set itemList to items 3 thru -1 of argv
set answer to choose from list itemList with title dialogTitle with prompt promptText OK button name "OK" cancel button name "Cancel"
if answer is false then error number -128
return item 1 of answer
end run"#;
    let output = Command::new("/usr/bin/osascript")
        .arg("-e")
        .arg(script)
        .arg("--")
        .arg(title)
        .arg(prompt)
        .args(options)
        .output()
        .ok()?;
    output
        .status
        .success()
        .then(|| clean_output(&output.stdout))
        .flatten()
}

#[cfg(target_os = "windows")]
fn choose_item_platform(title: &str, prompt: &str, options: &[String]) -> Option<String> {
    if options.is_empty() {
        return Some(String::new());
    }
    let script = r#"
Add-Type -AssemblyName System.Windows.Forms
$form = New-Object System.Windows.Forms.Form
$form.Text = $env:BGI_DIALOG_TITLE
$form.StartPosition = 'CenterScreen'
$form.Width = 520
$form.Height = 420
$label = New-Object System.Windows.Forms.Label
$label.Left = 12
$label.Top = 12
$label.Width = 480
$label.Height = 36
$label.Text = $env:BGI_DIALOG_PROMPT
$list = New-Object System.Windows.Forms.ListBox
$list.Left = 12
$list.Top = 52
$list.Width = 480
$list.Height = 280
$items = $env:BGI_DIALOG_OPTIONS.Split([char]31)
[void]$list.Items.AddRange($items)
$list.SelectedIndex = 0
$ok = New-Object System.Windows.Forms.Button
$ok.Text = 'OK'
$ok.Left = 332
$ok.Top = 338
$ok.Width = 75
$ok.DialogResult = [System.Windows.Forms.DialogResult]::OK
$cancel = New-Object System.Windows.Forms.Button
$cancel.Text = 'Cancel'
$cancel.Left = 417
$cancel.Top = 338
$cancel.Width = 75
$cancel.DialogResult = [System.Windows.Forms.DialogResult]::Cancel
$form.Controls.AddRange(@($label, $list, $ok, $cancel))
$form.AcceptButton = $ok
$form.CancelButton = $cancel
if ($form.ShowDialog() -eq [System.Windows.Forms.DialogResult]::OK) {
  [Console]::OutputEncoding = [System.Text.Encoding]::UTF8
  [Console]::Write([string]$list.SelectedItem)
  exit 0
}
exit 1
"#;
    for executable in ["powershell.exe", "pwsh.exe"] {
        let output = Command::new(executable)
            .args(["-NoProfile", "-STA", "-Command", script])
            .env("BGI_DIALOG_TITLE", title)
            .env("BGI_DIALOG_PROMPT", prompt)
            .env("BGI_DIALOG_OPTIONS", options.join("\u{1f}"))
            .output();
        if let Ok(output) = output {
            if output.status.success() {
                return Some(String::from_utf8_lossy(&output.stdout).to_string());
            }
            if output.status.code().is_some() {
                return None;
            }
        }
    }
    None
}

#[cfg(not(any(target_os = "macos", target_os = "windows")))]
fn choose_item_platform(title: &str, prompt: &str, options: &[String]) -> Option<String> {
    if options.is_empty() {
        return Some(String::new());
    }
    let output = Command::new("zenity")
        .arg("--list")
        .arg(format!("--title={title}"))
        .arg(format!("--text={prompt}"))
        .arg("--column=Selection")
        .args(options)
        .output();
    if let Ok(output) = output {
        if output.status.success() {
            return clean_output(&output.stdout);
        }
        if output.status.code().is_some() {
            return None;
        }
    }
    let initial = options.first().cloned().unwrap_or_default();
    let value = prompt_text_platform(title, prompt, &initial)?;
    options
        .iter()
        .find(|option| option.as_str() == value)
        .cloned()
}

pub(crate) fn choose_item(title: &str, prompt: &str, options: &[String]) -> Option<String> {
    choose_item_platform(title, prompt, options)
}

pub(crate) fn prompt_date_fields(
    title: &str,
    initial_fields: [String; 4],
    month_index: i32,
    day_index: i32,
) -> Option<([String; 4], i32, i32)> {
    let labels = ["Field 1", "Field 2", "Field 3", "Field 4"];
    let mut fields = initial_fields;
    for index in 0..4 {
        fields[index] = prompt_text(
            title,
            labels[index],
            &fields[index],
            10,
            TextInputConstraint::Any,
        )?;
    }
    let month = prompt_text(
        title,
        "Month (1-12)",
        &(month_index.clamp(0, 11) + 1).to_string(),
        2,
        TextInputConstraint::SignedDecimal,
    )?
    .parse::<i32>()
    .unwrap_or(1)
    .clamp(1, 12)
        - 1;
    let days = [31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
    let max_day = days[month as usize];
    let day = prompt_text(
        title,
        "Day",
        &(day_index.clamp(0, max_day - 1) + 1).to_string(),
        2,
        TextInputConstraint::SignedDecimal,
    )?
    .parse::<i32>()
    .unwrap_or(1)
    .clamp(1, max_day)
        - 1;
    Some((fields, month, day))
}

#[cfg(target_os = "macos")]
fn set_wallpaper_platform(path: &str, _style: i32, _tile: i32) -> bool {
    let script = r#"on run argv
set imagePath to item 1 of argv
tell application "System Events"
repeat with oneDesktop in desktops
set picture of oneDesktop to POSIX file imagePath
end repeat
end tell
end run"#;
    Command::new("/usr/bin/osascript")
        .arg("-e")
        .arg(script)
        .arg("--")
        .arg(path)
        .status()
        .map(|status| status.success())
        .unwrap_or(false)
}

#[cfg(target_os = "windows")]
fn set_wallpaper_platform(path: &str, style: i32, tile: i32) -> bool {
    let script = r#"
Set-ItemProperty -Path 'HKCU:\Control Panel\Desktop' -Name WallpaperStyle -Value $env:BGI_WALLPAPER_STYLE
Set-ItemProperty -Path 'HKCU:\Control Panel\Desktop' -Name TileWallpaper -Value $env:BGI_WALLPAPER_TILE
Add-Type @'
using System.Runtime.InteropServices;
public static class NativeWallpaper {
  [DllImport("user32.dll", SetLastError=true, CharSet=CharSet.Unicode)]
  public static extern bool SystemParametersInfo(uint action, uint param, string value, uint flags);
}
'@
if ([NativeWallpaper]::SystemParametersInfo(20, 0, $env:BGI_WALLPAPER_PATH, 3)) { exit 0 }
exit 1
"#;
    for executable in ["powershell.exe", "pwsh.exe"] {
        if let Ok(status) = Command::new(executable)
            .args(["-NoProfile", "-NonInteractive", "-Command", script])
            .env("BGI_WALLPAPER_PATH", path)
            .env("BGI_WALLPAPER_STYLE", style.to_string())
            .env("BGI_WALLPAPER_TILE", tile.to_string())
            .status()
        {
            return status.success();
        }
    }
    false
}

#[cfg(not(any(target_os = "macos", target_os = "windows")))]
fn set_wallpaper_platform(path: &str, style: i32, _tile: i32) -> bool {
    let uri = format!("file://{path}");
    let picture_option = match style {
        0 => "none",
        1 => "wallpaper",
        2 => "centered",
        3 => "scaled",
        4 => "stretched",
        _ => "zoom",
    };
    let gnome_ok = Command::new("gsettings")
        .args(["set", "org.gnome.desktop.background", "picture-uri", &uri])
        .status()
        .map(|status| status.success())
        .unwrap_or(false);
    if gnome_ok {
        let _ = Command::new("gsettings")
            .args([
                "set",
                "org.gnome.desktop.background",
                "picture-uri-dark",
                &uri,
            ])
            .status();
        let _ = Command::new("gsettings")
            .args([
                "set",
                "org.gnome.desktop.background",
                "picture-options",
                picture_option,
            ])
            .status();
        return true;
    }
    Command::new("feh")
        .args(["--bg-fill", path])
        .status()
        .map(|status| status.success())
        .unwrap_or(false)
}

pub(crate) fn set_wallpaper(path: &str, style: i32, tile: i32) -> bool {
    set_wallpaper_platform(path, style, tile)
}
