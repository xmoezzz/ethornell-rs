//! Portable presentation/input for the VM-owned BGI property inspector.
//! No guest address or executable window procedure enters the frontend.
use super::*;
pub(super) const PANEL_BITMAP:i32=0x5302;
use ethornell_vm::{NativeInspectorEvent, NativeInspectorView};

#[derive(Debug)]
pub(super) struct InspectorWindow {
    view: NativeInspectorView,
    visible: bool,
    focused: bool,
    scroll: usize,
    events: VecDeque<NativeInspectorEvent>,
    editor: Option<(usize, usize, String)>,
}
impl InspectorWindow {
    fn bounds(&self) -> [f32; 4] {
        [
            self.view.position[0] as f32,
            self.view.position[1] as f32,
            (self.view.columns[0].max(100)+self.view.columns[1].max(100)+32).clamp(300,2048) as f32,
            480.,
        ]
    }
}
#[derive(Debug, Default)]
pub(super) struct InspectorWindows {
    windows: BTreeMap<u32, InspectorWindow>,
    hover: Option<u32>,
}
impl InspectorWindows {
    pub(super) fn publish(&mut self, view: &NativeInspectorView) -> ethornell_vm::VmResult<bool> {
        if view.tabs.len() > 256
            || view.tabs.iter().any(|t| t.rows.len() > 4096)
            || view.title.len() > 65536
            || view.description.len() > 65536
        {
            return Err(ethornell_vm::VmError::Runtime(
                "native inspector presentation limit".into(),
            ));
        }
        match self.windows.get_mut(&view.handle) {
            Some(window) => window.view = view.clone(),
            None => {
                if self.windows.len() >= 16 {
                    return Ok(false);
                }
                self.windows.insert(
                    view.handle,
                    InspectorWindow {
                        view: view.clone(),
                        visible: true,
                        focused: true,
                        scroll: 0,
                        events: VecDeque::new(),
                        editor: None,
                    },
                );
            }
        }
        Ok(true)
    }
    pub(super) fn close(&mut self, handle: u32) {
        self.windows.remove(&handle);
    }
    pub(super) fn take_events(&mut self, handle: u32) -> Vec<NativeInspectorEvent> {
        self.windows
            .get_mut(&handle)
            .map(|w| w.events.drain(..).collect())
            .unwrap_or_default()
    }
    pub(super) fn focused(&self) -> bool {
        self.windows.values().any(|w| w.visible && w.focused)
    }
    pub(super) fn panels(&self)->Vec<[f32;4]> {
        self.windows.values().filter(|w|w.visible).map(|w|w.bounds()).collect()
    }
    pub(super) fn text_lines(&self) -> Vec<(String, f32, f32, f32)> {
        let mut lines = Vec::new();
        for w in self.windows.values().filter(|w| w.visible) {
            let [x, y, width, _] = w.bounds();
            lines.push((w.view.title.clone(),x,y,20.));
            lines.push(("[X]".into(),x+width-40.,y,20.));
            if !w.view.description.is_empty() {
                lines.push((w.view.description.clone(), x, y + 24., 14.));
            }
            for (n, tab) in w
                .view
                .tabs
                .iter()
                .enumerate()
                .skip(w.view.selected_tab.saturating_sub(2))
                .take(5)
            {
                let left = n.saturating_sub(w.view.selected_tab.saturating_sub(2));
                lines.push((
                    format!(
                        "{}{}",
                        if n == w.view.selected_tab { "> " } else { "  " },
                        tab.title
                    ),
                    x + left as f32 * 108.,
                    y + 48.,
                    16.,
                ));
            }
            if let Some(tab) = w.view.tabs.get(w.view.selected_tab) {
                for (row, p) in tab.rows.iter().enumerate().skip(w.scroll).take(12) {
                    let editor = w
                        .editor
                        .as_ref()
                        .filter(|(t, r, _)| *t == w.view.selected_tab && *r == row);
                    let value = editor
                        .map(|(_, _, s)| format!("{s}_"))
                        .unwrap_or_else(|| p.value.clone());
                    let py = y + 78. + (row - w.scroll) as f32 * 27.;
                    lines.push((p.name.clone(), x + 8., py, 16.));
                    lines.push((
                        format!("{}{value}", if p.editable { "[edit] " } else { "" }),
                        x + w.view.columns[0].max(100) as f32 + 16.,
                        py,
                        16.,
                    ));
                }
            }
            for (n, label) in w.view.buttons.iter().enumerate() {
                lines.push((format!("[ {label} ]"), x + n as f32 * 180., y + 432., 18.));
            }
        }
        lines
    }
    pub(super) fn pointer(&mut self, x: f32, y: f32, pressed: bool) -> bool {
        let hit = self
            .windows
            .iter()
            .rev()
            .find(|(_, w)| {
                let [left, top, width, height] = w.bounds();
                w.visible && x >= left && y >= top && x < left + width && y < top + height
            })
            .map(|(&h, _)| h);
        self.hover = hit;
        if pressed {
            for (&h, w) in &mut self.windows {
                w.focused = Some(h) == hit;
            }
        }
        let Some(handle) = hit else { return false };
        if !pressed {
            return true;
        }
        let w = self.windows.get_mut(&handle).unwrap();
        if w.events.len() >= 4096 {
            return true;
        }
        let [left, top, width, _] = w.bounds();
        let (x, y) = (x - left, y - top);
        if y < 24. && x > width-48. {
            w.visible = false;
            w.editor = None;
            w.events.push_back(NativeInspectorEvent::Close);
            return true;
        }
        if (44.0..74.0).contains(&y) {
            let index = (x / 108.) as usize + w.view.selected_tab.saturating_sub(2);
            if index < w.view.tabs.len() {
                w.view.selected_tab = index;
                w.scroll = 0;
                w.editor = None;
                w.events.push_back(NativeInspectorEvent::Tab(index));
            }
        } else if (74.0..402.0).contains(&y) {
            let index = ((y - 78.).max(0.) / 27.) as usize + w.scroll;
            if let Some(row) = w
                .view
                .tabs
                .get(w.view.selected_tab)
                .and_then(|t| t.rows.get(index))
            {
                w.events
                    .push_back(NativeInspectorEvent::Select(Some(index)));
                if x >= w.view.columns[0].max(100) as f32+8. && row.editable {
                    w.editor = Some((w.view.selected_tab, index, row.value.clone()));
                }
            }
        } else if (426.0..465.0).contains(&y) {
            let index = (x / 180.) as usize;
            if index < w.view.buttons.len() {
                w.events.push_back(NativeInspectorEvent::Button(index));
            }
        }
        true
    }
    pub(super) fn wheel(&mut self, delta: f32) -> bool {
        let Some(w) = self
            .hover
            .and_then(|h| self.windows.get_mut(&h))
            .filter(|w| w.visible)
        else {
            return false;
        };
        let max = w
            .view
            .tabs
            .get(w.view.selected_tab)
            .map_or(0, |t| t.rows.len().saturating_sub(12));
        if delta > 0. {
            w.scroll = w.scroll.saturating_sub(delta.ceil() as usize);
        } else {
            w.scroll = w.scroll.saturating_add((-delta).ceil() as usize).min(max);
        }
        true
    }
    pub(super) fn key(&mut self, key: KeyCode) -> bool {
        let Some(w) = self
            .windows
            .values_mut()
            .rev()
            .find(|w| w.visible && w.focused)
        else {
            return false;
        };
        if key == KeyCode::Escape {
            w.editor = None;
            return true;
        }
        let Some((tab, row, text)) = w.editor.as_mut() else {
            return false;
        };
        match key {
            KeyCode::Backspace => {
                text.pop();
                true
            }
            KeyCode::Enter | KeyCode::NumpadEnter => {
                let event = NativeInspectorEvent::Edit {
                    tab: *tab,
                    row: *row,
                    text: text.clone(),
                };
                if w.events.len() < 4096 {
                    w.events.push_back(event);
                }
                w.editor = None;
                true
            }
            _ => false,
        }
    }
    pub(super) fn text(&mut self, text: &str) -> bool {
        let Some((_, _, value)) = self
            .windows
            .values_mut()
            .rev()
            .find(|w| w.visible && w.focused)
            .and_then(|w| w.editor.as_mut())
        else {
            return false;
        };
        for ch in text.chars().filter(|c| !c.is_control()) {
            if value.len() + ch.len_utf8() <= 4096 {
                value.push(ch);
            }
        }
        true
    }
}
