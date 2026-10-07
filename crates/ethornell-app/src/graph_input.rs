use ethornell_vm::{GraphInputDescriptor, GraphInputRegion};
use std::collections::{BTreeMap, VecDeque};

#[derive(Debug, Clone)]
pub(crate) struct RuntimeGraphInputObject {
    pub(crate) layer: i32,
    pub(crate) registered_state: i32,
    pub(crate) descriptor: GraphInputDescriptor,
    /// Target DCIPIcon+0x80 (`this[32]`): raw hit-test item used by event
    /// 0x10000001. It is separate from pointer-current `hovered` (`this[29]`).
    pub(crate) raw_hit: Option<(i32, i32)>,
    pub(crate) hovered: Option<(i32, i32)>,
    pub(crate) queued_events: VecDeque<[i32; 3]>,
    pub(crate) indirect_messages: VecDeque<Vec<i32>>,
    pub(crate) extended: bool,
    pub(crate) item_states: BTreeMap<(i32, i32), i32>,
    /// Runtime per-group current/selected item.  This is the native group's
    /// mutable +0x08 field (base) / +0x0C field (extended), kept separate from
    /// the immutable BP descriptor.  Mouse hover is a different state.
    current_selections: BTreeMap<i32, i32>,
    /// Registry lifetime. A processor remains live until Graph90:B9 releases it.
    active: bool,
    /// Native DCIPIcon+0x30 (`this[12]`), returned as BC record word 0.
    /// Configure sets it to 1; a completed base activation makes the native
    /// per-tick handler return nonzero and sub_4485A0 then stores 0 here.
    running: bool,
    state_group: i32,
    state_region: i32,
    state_value: i32,
    state_local: (i32, i32),
    /// Target DCIPIcon+0x90 (`this[36]`): an item whose activation is
    /// deferred until mouse-left is released.  DCIPIconEx vtable+0x48
    /// (`sub_44C6F0`) does not disable these items; a false return stores the
    /// current live-item index here and `sub_448690` activates it on release
    /// only if the pointer is still over the same item.
    deferred_pointer_activation: Option<(i32, i32)>,
}

impl RuntimeGraphInputObject {
    pub(crate) fn new(layer: i32) -> Self {
        Self::new_with_variant(layer, false)
    }

    pub(crate) fn new_extended(layer: i32) -> Self {
        Self::new_with_variant(layer, true)
    }

    fn new_with_variant(layer: i32, extended: bool) -> Self {
        Self {
            layer,
            registered_state: 0,
            descriptor: GraphInputDescriptor::default(),
            raw_hit: None,
            hovered: None,
            queued_events: VecDeque::new(),
            indirect_messages: VecDeque::new(),
            extended,
            item_states: BTreeMap::new(),
            current_selections: BTreeMap::new(),
            active: false,
            running: false,
            state_group: -1,
            state_region: -1,
            state_value: 0,
            state_local: (0, 0),
            deferred_pointer_activation: None,
        }
    }

    pub(crate) fn configure(&mut self, descriptor: GraphInputDescriptor) {
        // sub_44A900 rebuilds the target descriptor storage and recomputes the
        // pointer-current item from a clean -1 state. Keeping an old hover over
        // reconfiguration suppresses the target's initial 0x10000002 event and
        // couples runtime state back into descriptor equality.
        self.raw_hit = None;
        self.hovered = None;
        self.registered_state = descriptor.initial_group;
        self.current_selections.clear();
        for group in &descriptor.groups {
            if group.initial_current_item >= 0 {
                self.current_selections
                    .insert(group.index, group.initial_current_item);
            }
        }
        // Keep hand-built/test descriptors compatible with the older
        // region.selected representation when no explicit group table exists.
        if descriptor.groups.is_empty() {
            for region in descriptor.regions.iter().filter(|region| region.selected) {
                self.current_selections.insert(region.group, region.index);
            }
        }
        // The descriptor's selected marker is only the configure-time value
        // copied from each native group record.  Target hover state lives in
        // DCIPIcon::m_pointer_item (this[29]) and never rewrites the descriptor.
        self.state_group = -1;
        self.state_region = -1;
        self.state_value = 0;
        self.state_local = (0, 0);
        self.deferred_pointer_activation = None;
        self.descriptor = descriptor;
        self.active = true;
        self.running = true;
        self.queued_events.clear();
        self.indirect_messages.clear();
        // Target sub_44A900 destroys and recreates the extended processor's
        // live child Sprites on every configure. Graph91:BB toggles those
        // children, not immutable descriptor bits, so prior runtime states do
        // not survive a descriptor rebuild.
        self.item_states.clear();
    }

    pub(crate) fn set_item_state(&mut self, group: i32, index: i32, state: i32) -> Result<(), i32> {
        if !self.extended {
            return Err(4);
        }
        let group_exists = self
            .descriptor
            .regions
            .iter()
            .any(|region| region.group == group);
        if !group_exists {
            return Err(2);
        }
        if !self
            .descriptor
            .regions
            .iter()
            .any(|region| region.group == group && region.index == index)
        {
            return Err(3);
        }
        self.item_states.insert((group, index), state);
        Ok(())
    }

    pub(crate) fn selected_region_values(&self) -> Vec<i32> {
        // Graph90:BE iterates the configured group table, not only groups that
        // happened to materialize an enabled region. Preserve zero-item and
        // fully-disabled groups as explicit -1 entries.
        let group_count = if self.descriptor.groups.is_empty() {
            self.descriptor
                .regions
                .iter()
                .map(|region| region.group)
                .max()
                .and_then(|group| usize::try_from(group.saturating_add(1)).ok())
                .unwrap_or_default()
        } else {
            self.descriptor.groups.len()
        };
        let mut values = vec![-1; group_count];
        for (&group, &index) in &self.current_selections {
            if let Ok(group) = usize::try_from(group)
                && let Some(value) = values.get_mut(group)
            {
                *value = index;
            }
        }
        values
    }

    pub(crate) fn is_current_selection(&self, group: i32, index: i32) -> bool {
        self.current_selections.get(&group).copied() == Some(index)
    }

    /// Root+0x0C/+0x10 enable sampling the owning Window's input scope.
    /// With both clear, only the separate live-item Virtual scope is sampled.
    /// amachoco.exe sub_451720 copies these to DCIPIcon+0x40/+0x44;
    /// sub_44F2E0 skips the Window query when both are zero. In particular,
    /// an icon toolbar's empty rectangle must not consume dialogue clicks.
    pub(crate) fn samples_window_pointer_input(&self) -> bool {
        self.descriptor.pointer_processing_enabled
            && (self.descriptor.flags[0] != 0 || self.descriptor.flags[1] != 0)
    }

    /// Graph91:BB reaches sub_44B460 -> sub_42C060 -> CDspObj::SetEnabled on
    /// the materialized child Sprite. Unmentioned items keep the constructor
    /// default enabled=1.
    pub(crate) fn item_enabled(&self, group: i32, index: i32) -> bool {
        self.item_states.get(&(group, index)).copied().unwrap_or(1) != 0
    }

    /// Target DCIPIconEx hit selection calls vtable+0x28 (sub_44C110) before
    /// accepting a materialized child. Extended source item+0x34 excludes an
    /// item only while that item is the group's current selection.
    pub(crate) fn pointer_hit_eligible(&self, region: GraphInputRegion) -> bool {
        self.item_enabled(region.group, region.index)
            && !(self.extended
                && region.current_selection_hit_excluded
                && self.is_current_selection(region.group, region.index))
    }

    pub(crate) fn group_pointer_selection_enabled(&self, group: i32) -> bool {
        self.descriptor.pointer_processing_enabled
            && self
                .descriptor
                .groups
                .iter()
                .find(|candidate| candidate.index == group)
                .map(|candidate| candidate.selection_enabled && candidate.pointer_selection_enabled)
                // Legacy hand-built descriptors have no group table. Preserve
                // their former permissive behavior only for tests/compatibility.
                .unwrap_or(self.descriptor.groups.is_empty())
    }

    /// Target internal group+0x14. This is not a fresh-click enable flag.
    /// `sub_448690` consults it only after the normal edge state has already
    /// been sampled: while mouse-left remains held, a nonzero value allows
    /// `sub_46E490() & 1` to OR action bit 1 back into DCIPIcon+0x8C.
    pub(crate) fn group_pointer_activation_enabled(&self, group: i32) -> bool {
        self.descriptor.pointer_processing_enabled
            && self
                .descriptor
                .groups
                .iter()
                .find(|candidate| candidate.index == group)
                .map(|candidate| candidate.pointer_activation_enabled)
                .unwrap_or(self.descriptor.groups.is_empty())
    }

    /// Whether target action 1 activates this item immediately on MouseDown.
    /// Base DCIPIcon always does. DCIPIconEx vtable+0x48 (`sub_44C6F0`)
    /// returns true when source group+0x3C bit 0x02 and item+0xC0 bit 0x20 are
    /// both clear. A false return is *not* a disabled-item result: `sub_448690`
    /// stores the hit in DCIPIcon+0x90 (`this[36]`) and, after mouse-left is
    /// released, re-enters action 1 if the pointer still hits the same item.
    /// Group+0x14 is intentionally absent because it only controls held-button
    /// action reinjection.
    pub(crate) fn pointer_activation_is_immediate(&self, region: GraphInputRegion) -> bool {
        if !self.extended {
            return true;
        }
        let group_flags = self
            .descriptor
            .groups
            .iter()
            .find(|candidate| candidate.index == region.group)
            .map(|candidate| candidate.extended_flags)
            .unwrap_or_default();
        group_flags & 0x02 == 0 && region.flags & 0x20 == 0
    }

    pub(crate) fn defer_pointer_activation(&mut self, region: GraphInputRegion) {
        self.deferred_pointer_activation = Some((region.group, region.index));
    }

    pub(crate) fn deferred_pointer_activation(&self) -> Option<(i32, i32)> {
        self.deferred_pointer_activation
    }

    pub(crate) fn clear_deferred_pointer_activation(&mut self) {
        self.deferred_pointer_activation = None;
    }

    /// Map one fresh physical mouse-left edge through the root input action
    /// configuration from target `sub_448690`.
    ///
    /// Root+0x14 is copied to DCIPIcon+0x48 and suppresses physical input bits;
    /// root+0x18 selects one of eight 24-entry action tables. For built-in
    /// modes 0..=3, physical mouse-left maps to action 1. Modes 4..=7 use the
    /// tables registered by Graph91:BF; an unregistered target table is zero.
    pub(crate) fn map_fresh_mouse_left_action(
        &self,
        key_assignments: &[Option<[i32; 24]>; 4],
    ) -> i32 {
        const MOUSE_LEFT_BIT: i32 = 1;
        if self.descriptor.flags[2] & MOUSE_LEFT_BIT != 0 {
            return 0;
        }
        let mode = (self.descriptor.flags[3] & 7) as usize;
        if mode < 4 {
            1
        } else {
            key_assignments[mode - 4]
                .as_ref()
                .map(|table| table[0])
                .unwrap_or(0)
        }
    }

    /// Target sub_449A60 + sub_449BC0 pointer-driven current-item change.
    /// Returns true only when the group/item state actually changes and the
    /// group is eligible for pointer selection.
    pub(crate) fn select_pointer_item(&mut self, group: i32, index: i32) -> bool {
        if !self.group_pointer_selection_enabled(group)
            || self.current_selections.get(&group).copied() == Some(index)
        {
            return false;
        }

        let exclusion_key = self
            .descriptor
            .groups
            .iter()
            .find(|candidate| candidate.index == group)
            .map(|candidate| candidate.selection_exclusion_key)
            .unwrap_or(-1);
        if exclusion_key != -1 {
            let peers = self
                .descriptor
                .groups
                .iter()
                .filter(|candidate| {
                    candidate.index != group && candidate.selection_exclusion_key == exclusion_key
                })
                .map(|candidate| candidate.index)
                .collect::<Vec<_>>();
            for peer in peers {
                self.current_selections.remove(&peer);
            }
        }

        self.registered_state = group;
        self.current_selections.insert(group, index);
        true
    }

    pub(crate) fn queue_event(&mut self, event: [i32; 3]) {
        self.queued_events.push_back(event);
    }

    pub(crate) fn pop_event(&mut self) -> Option<[i32; 3]> {
        self.queued_events.pop_front()
    }

    pub(crate) fn queue_indirect_message(&mut self, values: Vec<i32>) {
        self.indirect_messages.push_back(values);
    }

    pub(crate) fn complete(&mut self, region: GraphInputRegion, local_x: i32, local_y: i32) {
        // Target sub_449FA0/sub_44C170 update item state but do not deactivate
        // the DCIPIcon/DCIPIconEx processor.  It remains live until the
        // explicit GraphReleaseIconInputProcessor path removes it.
        self.state_group = region.group;
        self.state_region = region.index;
        self.state_value = 1;
        self.state_local = (local_x, local_y);
        if !self.extended {
            self.running = false;
        }
    }

    pub(crate) fn observe_hit(&mut self, hit: Option<(GraphInputRegion, i32, i32)>) {
        if !self.active {
            return;
        }
        // Target sub_449760/sub_4497A0 keep pointer-hit state separate from
        // group selection and activation state.  In particular, plain hover
        // must not rewrite descriptor.selected, registered_state, or BC state.
        self.hovered = hit.map(|(region, _, _)| (region.group, region.index));
    }

    pub(crate) fn begin_interaction(
        &mut self,
        region: GraphInputRegion,
        local_x: i32,
        local_y: i32,
    ) {
        if !self.active {
            return;
        }
        self.state_group = region.group;
        self.state_region = region.index;
        self.state_value = 1;
        self.state_local = (local_x, local_y);
        if !self.extended {
            self.running = false;
        }
    }

    pub(crate) fn is_running(&self) -> bool {
        self.active && self.running
    }

    pub(crate) fn state_record(&self) -> [i32; 6] {
        let (local_x, local_y) = if self.state_value != 0 {
            self.state_local
        } else {
            (0, 0)
        };
        [
            i32::from(self.running),
            self.state_group,
            self.state_region,
            self.state_value,
            local_x,
            local_y,
        ]
    }

    pub(crate) fn hit_test(&self, point: (f32, f32)) -> Option<(GraphInputRegion, i32, i32)> {
        self.descriptor
            .regions
            .iter()
            .filter(|region| region.enabled_depth != 0)
            .filter(|region| self.pointer_hit_eligible(**region))
            .filter_map(|region| {
                let left = region.x as f32;
                let top = region.y as f32;
                let right = left + region.width as f32;
                let bottom = top + region.height as f32;
                (point.0 >= left && point.0 < right && point.1 >= top && point.1 < bottom).then(
                    || {
                        (
                            *region,
                            (point.0 - left).round() as i32,
                            (point.1 - top).round() as i32,
                        )
                    },
                )
            })
            .max_by_key(|(region, _, _)| native_region_depth(region))
    }
}

fn native_region_depth(region: &GraphInputRegion) -> i32 {
    if region.flags & 0x10 != 0 {
        region.enabled_depth
    } else if region.flags & 0x02 != 0 {
        region.y
    } else {
        region.ordinal
    }
}

pub(crate) fn pack_words(high: i32, low: i32) -> i32 {
    ((high & 0xffff) << 16) | (low & 0xffff)
}

#[cfg(test)]
mod tests {
    use super::RuntimeGraphInputObject;
    use ethornell_vm::{GraphInputDescriptor, GraphInputGroup, GraphInputRegion};

    fn region() -> GraphInputRegion {
        GraphInputRegion {
            group: 0,
            index: 1,
            ordinal: 0,
            enabled_depth: 1,
            selected: false,
            x: 10,
            y: 20,
            width: 100,
            height: 50,
            normal_resource: 0,
            selected_resource: 0,
            hover_resource: -1,
            hover_selected_resource: -1,
            mask_resource: -1,
            current_selection_hit_excluded: false,
            flags: 0,
        }
    }

    #[test]
    fn native_state_stays_active_after_a_region_completes() {
        let mut input = RuntimeGraphInputObject::new(7);
        input.configure(GraphInputDescriptor {
            initial_group: 0,
            regions: vec![region()],
            ..GraphInputDescriptor::default()
        });

        assert_eq!(input.state_record(), [1, -1, -1, 0, 0, 0]);
        input.observe_hit(input.hit_test((20.0, 30.0)));
        assert_eq!(
            input.state_record(),
            [1, -1, -1, 0, 0, 0],
            "pointer hover is separate from Graph90:BC activation state"
        );

        input.begin_interaction(region(), 10, 10);
        assert_eq!(input.state_record(), [0, 0, 1, 1, 10, 10]);

        input.complete(region(), 10, 10);
        assert_eq!(
            input.state_record(),
            [0, 0, 1, 1, 10, 10],
            "registry lifetime is separate from the BC running flag"
        );
    }

    #[test]
    fn extended_item_state_reports_target_status_classes() {
        let mut base = RuntimeGraphInputObject::new(7);
        base.configure(GraphInputDescriptor {
            initial_group: 0,
            regions: vec![region()],
            ..GraphInputDescriptor::default()
        });
        assert_eq!(base.set_item_state(0, 1, 3), Err(4));

        let mut extended = RuntimeGraphInputObject::new_extended(7);
        extended.configure(GraphInputDescriptor {
            initial_group: 0,
            regions: vec![region()],
            ..GraphInputDescriptor::default()
        });
        assert_eq!(extended.set_item_state(9, 1, 3), Err(2));
        assert_eq!(extended.set_item_state(0, 9, 3), Err(3));
        assert_eq!(extended.set_item_state(0, 1, 3), Ok(()));
        assert_eq!(extended.item_states.get(&(0, 1)), Some(&3));
    }

    #[test]
    fn selected_region_query_returns_one_index_per_group() {
        let mut input = RuntimeGraphInputObject::new(7);
        let mut first = region();
        first.group = 0;
        first.index = 0;
        first.selected = true;
        let mut second = region();
        second.group = 1;
        second.index = 2;
        second.selected = true;
        input.configure(GraphInputDescriptor {
            initial_group: 1,
            regions: vec![first, second],
            ..GraphInputDescriptor::default()
        });

        assert_eq!(input.registered_state, 1);
        assert_eq!(input.selected_region_values(), [0, 2]);
    }

    #[test]
    fn extended_activation_timing_is_independent_from_hoverability() {
        let mut input = RuntimeGraphInputObject::new_extended(7);
        let mut item = region();
        item.hover_resource = 11;
        input.configure(GraphInputDescriptor {
            groups: vec![GraphInputGroup {
                index: 0,
                initial_current_item: -1,
                selection_enabled: true,
                pointer_selection_enabled: true,
                pointer_activation_enabled: true,
                selection_exclusion_key: -1,
                extended_flags: 0x02,
            }],
            regions: vec![item],
            ..GraphInputDescriptor::default()
        });

        assert_eq!(input.hit_test((20.0, 30.0)).map(|hit| hit.0.index), Some(1));
        assert!(!input.pointer_activation_is_immediate(item));

        input.descriptor.groups[0].extended_flags = 0;
        item.flags = 0x20;
        assert!(!input.pointer_activation_is_immediate(item));

        item.flags = 0;
        assert!(input.pointer_activation_is_immediate(item));

        input.defer_pointer_activation(item);
        assert_eq!(
            input.deferred_pointer_activation(),
            Some((item.group, item.index))
        );
        input.clear_deferred_pointer_activation();
        assert_eq!(input.deferred_pointer_activation(), None);
    }
}
