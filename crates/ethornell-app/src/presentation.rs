//! Native scene presentation and global movie samples have separate producers.
//! Tayutama sub_48EF80 installs DCTraditionalVR; sub_45B860 presents each
//! delivered sample directly, while sub_461450 excludes ordinary scene drawing
//! during global playback. DCVideoImageSender (sub_45BD00) only updates a
//! bitmap and posts its event, so bitmap movies do not bypass Graph90:01.
use super::*;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
struct MovieStamp {
    handle: i32,
    revision: Option<u64>,
    size: (i32, i32),
    canvas: (i32, i32),
}

#[derive(Clone, Copy)]
pub(super) struct FramePlan {
    pub(super) refresh_scene: bool,
    pub(super) present: bool,
    pub(super) movie_active: bool,
    movie: Option<MovieStamp>,
}

#[derive(Default)]
pub(super) struct FrameSchedule {
    movie: Option<MovieStamp>,
    initialized: bool,
}

impl FrameSchedule {
    pub(super) fn plan(&self, api: &mut RuntimeTraceApi) -> FramePlan {
        let movie = api.active_global_movie_handle().and_then(|handle| {
            (api.graph_effects.state(handle) == 0).then(|| {
                let key = format!("runtime:movie:{handle}");
                MovieStamp {
                    handle,
                    revision: api
                        .graph_images
                        .contains_key(&key)
                        .then(|| api.graph_image_revision(&key)),
                    size: if api.version_graph.mf_handle==Some(handle) {(0,0)} else {api.global_movie_size},
                    canvas: (api.screen_width, api.screen_height),
                }
            })
        });
        // Do not consume the scene deadline while its presenter is suspended.
        let refresh_scene = movie.is_none() && (api.take_present_due() || !self.initialized);
        FramePlan {
            refresh_scene,
            present: refresh_scene || !self.initialized || movie != self.movie,
            movie_active: movie.is_some(),
            movie,
        }
    }

    pub(super) fn commit(&mut self, plan: FramePlan) {
        self.movie = plan.movie;
        self.initialized = true;
    }
}

/// CPU equivalent of the GPU presenter's retained scene and independent video
/// plane. Only a new sample/state or a due scene frame triggers composition.
#[derive(Default)]
pub(super) struct CpuPresenter {
    schedule: FrameSchedule,
    scene: Option<snapshot::RuntimeFrameComposition>,
    output: Option<snapshot::RuntimeFrameComposition>,
}

impl CpuPresenter {
    pub(super) fn frame(
        &mut self,
        api: &mut RuntimeTraceApi,
    ) -> &snapshot::RuntimeFrameComposition {
        let plan = self.schedule.plan(api);
        if plan.present {
            api.performance_profile.begin();
        }
        if plan.refresh_scene {
            self.scene = Some(snapshot::compose_runtime_frame_with_diagnostics(api, true));
        }
        if plan.present {
            self.output = Some(if plan.movie_active || self.scene.is_none() {
                let framebuffer = snapshot::compose_global_movie_frame(api);
                let framebuffer_hash = snapshot::stable_framebuffer_hash(&framebuffer);
                snapshot::RuntimeFrameComposition {
                    framebuffer,
                    framebuffer_hash,
                    render_tree_nodes: usize::from(api.directshow_movie_draw_item().is_some()),
                }
            } else {
                self.scene.as_ref().expect("retained scene").clone()
            });
            self.schedule.commit(plan);
            api.performance_profile.end(true);
        }
        self.output.as_ref().expect("initial presentation")
    }
}
