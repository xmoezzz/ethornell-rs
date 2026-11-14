//! BGI 1.687 D0:C0..C8 (1400AA060..1400AA420): directed, spatially weighted
//! route networks. Nodes and edges are process state, shared by BP threads.
use crate::{NativeAbiProfile, Value, Vm, VmError, VmResult};
use std::collections::{BTreeMap, VecDeque};

#[derive(Debug, Default)]
pub(super) struct Networks {
    next: i32,
    entries: BTreeMap<i32, Network>,
}
#[derive(Debug, Default)]
struct Network {
    attributes: u32,
    nodes: Vec<Option<Node>>,
}
#[derive(Debug)]
struct Node {
    position: [f32; 4],
    edges: BTreeMap<i32, Edge>,
}
#[derive(Debug)]
struct Edge {
    cost: f32,
    attributes: BTreeMap<u32, f32>,
}

impl Network {
    fn node(&self, id: i32) -> Option<&Node> {
        self.nodes.get(usize::try_from(id).ok()?)?.as_ref()
    }
    fn node_mut(&mut self, id: i32) -> Option<&mut Node> {
        self.nodes.get_mut(usize::try_from(id).ok()?)?.as_mut()
    }
    /// 1400A9CD0 / A9BA0: FIFO relaxation; use the queued cost, not the
    /// latest node cost. Equal costs retain the first predecessor.
    fn route(&self, start: i32, target: i32, attribute: i32) -> VmResult<Result<Vec<i32>, i32>> {
        if start < 0 || start as usize >= self.nodes.len() {
            return Ok(Err(2));
        }
        if target < 0 || target as usize >= self.nodes.len() {
            return Ok(Err(6));
        }
        if self.node(start).is_none() {
            return Ok(Err(2));
        }
        let mut costs = vec![0f32; self.nodes.len()];
        let mut previous = vec![-1; self.nodes.len()];
        let mut queue = VecDeque::from([(start, 0f32)]);
        let mut visits = 0usize;
        while let Some((index, cost)) = queue.pop_front() {
            visits += 1;
            if visits > 1_048_576 {
                // Native zero-cost cycles can grow its list indefinitely.
                return Err(VmError::Runtime(
                    "native network relaxation limit (zero-cost cycle)".into(),
                ));
            }
            let node = self.node(index).unwrap();
            for (&to, edge) in &node.edges {
                let Some(destination) = self.node(to) else {
                    continue;
                };
                let delta: [f32; 4] = std::array::from_fn(|n| {
                    let d = destination.position[n] - node.position[n];
                    d * d
                });
                // Two SSE HADDs include the fourth position lane.
                let distance = ((delta[0] + delta[1]) + (delta[2] + delta[3])).sqrt();
                let factor = edge
                    .attributes
                    .get(&(attribute as u32))
                    .copied()
                    .unwrap_or(edge.cost);
                let next = factor * distance + cost;
                let slot = to as usize;
                if costs[slot] > next || costs[slot] == 0.0 {
                    costs[slot] = next;
                    previous[slot] = index;
                    queue.push_back((to, next));
                }
            }
        }
        if costs[target as usize] <= 0.0 {
            return Ok(Err(6));
        }
        let mut path = Vec::new();
        let mut at = target;
        while at != start {
            if at < 0 || path.len() > self.nodes.len() {
                return Err(VmError::Runtime("native network predecessor cycle".into()));
            }
            path.push(at);
            at = previous[at as usize];
        }
        path.reverse();
        Ok(Ok(path))
    }
}

impl Vm {
    pub(crate) fn execute_network(&mut self, id: u8) -> VmResult<Option<Value>> {
        if self.native_abi_profile != NativeAbiProfile::ModernFused {
            return Ok(None);
        }
        let argc = match id {
            0xc0 | 0xc4 | 0xc7 => 3,
            0xc1 | 0xc2 => 1,
            0xc5 => 2,
            0xc6 | 0xc8 => 6,
            _ => return Ok(None),
        };
        let mut a = Vec::with_capacity(argc);
        for _ in 0..argc {
            a.push(self.pop_int()?);
        }
        a.reverse();
        let result = match id {
            0xc0 => {
                // FCA10 publishes the new handle even when AA420 rejects setup.
                self.logic.networks.next = self.logic.networks.next.wrapping_add(1);
                let handle = self.logic.networks.next;
                self.write_int(a[0] as u32, 2, handle as u32)?;
                let graph = self.logic.networks.entries.entry(handle).or_default();
                if !(1..=1024).contains(&a[1]) {
                    2
                } else if a[2] == 1 || a[2] as u32 > 32 {
                    5
                } else {
                    graph.attributes = a[2] as u32;
                    graph.nodes = (0..a[1]).map(|_| None).collect();
                    0
                }
            }
            0xc1 => {
                if self.logic.networks.entries.remove(&a[0]).is_some() {
                    0
                } else {
                    1
                }
            }
            0xc2 => {
                if let Some(graph) = self.logic.networks.entries.get_mut(&a[0]) {
                    if graph.nodes.is_empty() {
                        2
                    } else {
                        for n in &mut graph.nodes {
                            *n = None;
                        }
                        0
                    }
                } else {
                    1
                }
            }
            0xc4 => {
                // A null native vector creates the node at the origin.
                let position = if a[2] == 0 {
                    [0.; 4]
                } else {
                    let raw = self.logic_read_i32s(a[2] as u32, 4)?;
                    std::array::from_fn(|n| raw[n] as f32 / 65536.)
                };
                match self.logic.networks.entries.get_mut(&a[0]) {
                    None => 1,
                    Some(graph) => match usize::try_from(a[1])
                        .ok()
                        .and_then(|n| graph.nodes.get_mut(n))
                    {
                        None => 2,
                        Some(slot) => {
                            *slot = Some(Node {
                                position,
                                edges: BTreeMap::new(),
                            });
                            0
                        }
                    },
                }
            }
            0xc5 => match self.logic.networks.entries.get_mut(&a[0]) {
                None => 1,
                Some(graph) => {
                    if graph.node_mut(a[1]).is_none() {
                        2
                    } else {
                        graph.nodes[a[1] as usize] = None;
                        0
                    }
                }
            },
            0xc6 => {
                let mut attributes = BTreeMap::new();
                if a[4] > 0 {
                    if a[4] > 1_048_576 {
                        return Err(VmError::Runtime(
                            "native network attribute count limit".into(),
                        ));
                    }
                    for word in self.logic_read_i32s(a[5] as u32, a[4] as usize)? {
                        attributes.insert(word as u32 & 255, (word as u32 >> 8) as f32 / 65536.);
                    }
                }
                match self.logic.networks.entries.get_mut(&a[0]) {
                    None => 1,
                    Some(graph) if graph.node(a[1]).is_none() => 2,
                    Some(graph) if a[2] < 0 || a[2] as usize >= graph.nodes.len() => 3,
                    Some(_) if a[3] <= 0 => 4,
                    Some(graph) if attributes.keys().any(|&k| k >= graph.attributes) => 5,
                    Some(_) if attributes.values().any(|&v| v <= 0.) => 4,
                    Some(graph) => {
                        graph.node_mut(a[1]).unwrap().edges.insert(
                            a[2],
                            Edge {
                                cost: a[3] as f32 / 65536.,
                                attributes,
                            },
                        );
                        0
                    }
                }
            }
            0xc7 => match self.logic.networks.entries.get_mut(&a[0]) {
                None => 1,
                Some(graph) => match graph.node_mut(a[1]) {
                    None => 2,
                    Some(node) => {
                        if node.edges.remove(&a[2]).is_some() {
                            0
                        } else {
                            3
                        }
                    }
                },
            },
            0xc8 => match self.logic.networks.entries.get(&a[2]) {
                None => 1,
                Some(graph) => match graph.route(a[3], a[4], a[5])? {
                    Err(status) => status,
                    Ok(path) => {
                        // Validate both outputs before publishing either one.
                        self.resolve_write_range(a[0] as u32, 4)?;
                        self.resolve_write_range(a[1] as u32, path.len() * 4)?;
                        self.logic_write_i32(a[0] as u32, path.len() as i32)?;
                        self.logic_write_i32s(a[1] as u32, &path)?;
                        0
                    }
                },
            },
            _ => unreachable!(),
        };
        Ok(Some(Value::Int(result)))
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn version_network_routes_attribute_cost_and_node_replacement() {
        fn call(vm: &mut Vm, id: u8, args: &[i32]) -> i32 {
            vm.stack = args.iter().copied().map(Value::Int).collect();
            vm.execute_network(id).unwrap().unwrap().as_i32()
        }
        let mut vm = Vm::new();
        vm.set_native_abi_profile(NativeAbiProfile::ModernFused);
        assert_eq!(call(&mut vm, 0xc0, &[256, 4, 2]), 0);
        let h = vm.read_int(256, 2).unwrap() as i32;
        for (i, x, y) in [(0, 0, 0), (1, 1, 1), (2, 2, 0), (3, 3, 0)] {
            for (n, v) in [x << 16, y << 16, 0, 0].into_iter().enumerate() {
                vm.write_int(512 + 4 * n as u32, 2, v as u32).unwrap();
            }
            assert_eq!(call(&mut vm, 0xc4, &[h, i, 512]), 0);
        }
        vm.write_int(600, 2, 10 * 65536 * 256 + 1).unwrap();
        for (from, to) in [(0, 1), (1, 2), (0, 2), (2, 3)] {
            assert_eq!(
                call(
                    &mut vm,
                    0xc6,
                    &[h, from, to, 65536, i32::from(from == 0 && to == 2), 600]
                ),
                0
            );
        }
        assert_eq!(call(&mut vm, 0xc8, &[700, 704, h, 0, 3, 0]), 0);
        assert_eq!(vm.read_int(700, 2).unwrap(), 2);
        assert_eq!(vm.read_int(704, 2).unwrap(), 2);
        assert_eq!(call(&mut vm, 0xc8, &[700, 704, h, 0, 3, 1]), 0);
        assert_eq!(vm.read_int(700, 2).unwrap(), 3);
        assert_eq!(vm.read_int(704, 2).unwrap(), 1);
        // Replacing a node destroys its outgoing edges; incoming edges survive.
        assert_eq!(call(&mut vm, 0xc4, &[h, 2, 0]), 0);
        vm.write_int(700, 2, 99).unwrap();
        assert_eq!(call(&mut vm, 0xc8, &[700, 704, h, 0, 3, 0]), 6);
        assert_eq!(vm.read_int(700, 2).unwrap(), 99);
        assert_eq!(call(&mut vm, 0xc1, &[h]), 0);
        assert_eq!(call(&mut vm, 0xc2, &[h]), 1);
    }
}
