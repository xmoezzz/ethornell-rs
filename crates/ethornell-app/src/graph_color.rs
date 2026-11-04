#[derive(Clone, Debug)]
pub(crate) struct RuntimeColorLut {
    channels: [[u8; 256]; 3],
}

impl RuntimeColorLut {
    pub(crate) fn from_control_points(points: [[i32; 2]; 3]) -> Option<Self> {
        if points
            .iter()
            .any(|point| !(1..255).contains(&point[0]) || !(0..=255).contains(&point[1]))
        {
            return None;
        }
        // sub_40A060 builds the B table (+2052) from the last pair, G
        // (+1028) from the middle one and R (+4) from the first: the
        // tables are kept in BGRA lane order. Matched with 90:CD under
        // Unicorn (bitmap_kernels vectors).
        let points = [points[2], points[1], points[0]];
        Some(Self {
            channels: points.map(|point| spline_channel(point[0], point[1])),
        })
    }

    /// The three lookup tables of the target record, by BGRA lane:
    /// [B (+2052, point 2), G (+1028, point 1), R (+4, point 0)].
    pub(crate) fn lane_tables(&self) -> &[[u8; 256]; 3] {
        &self.channels
    }
}

fn spline_channel(control_x: i32, control_y: i32) -> [u8; 256] {
    let mut result = [0_u8; 256];
    let knot_x = [0.0, f64::from(control_x), 255.0];
    let knot_y = [0.0, f64::from(control_y), 255.0];
    let widths = [knot_x[1] - knot_x[0], knot_x[2] - knot_x[1]];
    let mut curvature = [0.0_f64; 3];
    curvature[1] = (((knot_y[2] - knot_y[1]) / widths[1]) - ((knot_y[1] - knot_y[0]) / widths[0]))
        * 3.0
        / ((widths[1] + widths[0]) * 2.0);

    for (input, output) in result.iter_mut().enumerate() {
        let input = input as f64;
        let segment = usize::from(input >= knot_x[1]);
        let offset = input - knot_x[segment];
        let value = offset
            * ((knot_y[segment + 1] - knot_y[segment]) / widths[segment]
                - (2.0 * curvature[segment] + curvature[segment + 1]) * widths[segment] / 3.0
                + (((curvature[segment + 1] - curvature[segment]) / (3.0 * widths[segment]))
                    * offset
                    + curvature[segment])
                    * offset)
            + knot_y[segment];
        *output = native_round(value).clamp(0, 255) as u8;
    }
    result
}

fn native_round(value: f64) -> i32 {
    if value < 0.0 {
        (value - 0.5) as i32
    } else {
        (value + 0.5) as i32
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn native_identity_control_points_are_identity_tables() {
        let lut = RuntimeColorLut::from_control_points([[128, 128]; 3]).unwrap();
        for table in lut.lane_tables() {
            assert!(table.iter().enumerate().all(|(index, &value)| usize::from(value) == index));
        }
    }

    #[test]
    fn native_curve_descriptor_rejects_endpoint_control_x() {
        assert!(RuntimeColorLut::from_control_points([[0, 0], [128, 128], [128, 128]]).is_none());
        assert!(
            RuntimeColorLut::from_control_points([[255, 255], [128, 128], [128, 128]]).is_none()
        );
    }
}
