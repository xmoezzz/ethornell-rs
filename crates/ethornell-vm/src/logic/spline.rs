//! `CSpline` (0x444230..0x444AF0): a 3D path through up to 100 points, one
//! natural cubic spline per axis, sampled by tick. The 0xD0 line-of-fire test
//! (`sub_455360`) is its only user here. x87 runs at 53-bit precision under the
//! MSVC CRT, so each step below is one f64 operation in the target's order.

const MAX_POINTS: usize = 100;

/// One axis (`sub_444230` solves, `sub_4443E0` evaluates). With n = points-1,
/// segment k is `((d·u + c)·u + b)·u + a` for u in [0, 1).
#[derive(Debug, Clone)]
struct Axis {
    a: Vec<f64>,
    b: Vec<f64>,
    c: Vec<f64>,
    d: Vec<f64>,
}

impl Axis {
    fn solve(points: &[i32]) -> Self {
        let n = points.len() - 1;
        let a: Vec<f64> = points.iter().map(|&p| f64::from(p)).collect();
        let mut c = vec![0.0; n + 1];
        for i in 1..n {
            c[i] = (a[i - 1] - a[i] * 2.0 + a[i + 1]) * 3.0;
        }
        // Thomas sweep for the 1-4-1 system; upper[i] is the target's local
        // array one slot ahead (var_330 + 8i).
        let mut upper = vec![0.0; n + 2];
        for i in 1..n {
            let denominator = 4.0 - upper[i];
            c[i] = (c[i] - c[i - 1]) / denominator;
            upper[i + 1] = 1.0 / denominator;
        }
        for k in (1..n).rev() {
            c[k] -= upper[k + 1] * c[k + 1];
        }
        let mut b = vec![0.0; n + 1];
        let mut d = vec![0.0; n + 1];
        for i in 0..n {
            d[i] = (c[i + 1] - c[i]) / 3.0;
            b[i] = a[i + 1] - a[i] - c[i] - d[i];
        }
        Self { a, b, c, d }
    }

    fn eval(&self, t: f64) -> f64 {
        let n = (self.a.len() - 1) as i32;
        let x = f64::from(n) * t;
        let mut k = x.floor() as i32;
        if k < 0 {
            k = 0;
        } else if k >= n {
            k = n - 1;
        }
        let u = x - f64::from(k);
        let k = k as usize;
        ((self.d[k] * u + self.c[k]) * u + self.b[k]) * u + self.a[k]
    }
}

#[derive(Debug, Clone, Default)]
pub(crate) struct CSpline {
    points: [Vec<i32>; 3],
    start: u32,
    duration: u32,
    solved: Option<[Axis; 3]>,
}

impl CSpline {
    /// `sub_444650`: false once 100 points are stored.
    pub(crate) fn add(&mut self, x: i32, y: i32, z: i32) -> bool {
        if self.points[0].len() >= MAX_POINTS {
            return false;
        }
        self.solved = None;
        for (axis, value) in self.points.iter_mut().zip([x, y, z]) {
            axis.push(value);
        }
        true
    }

    /// `sub_4446A0`: the path runs from tick 0 for `duration` ticks.
    pub(crate) fn set_duration(&mut self, duration: u32) {
        self.start = 0;
        self.duration = duration;
    }

    /// `sub_4446C0`: the point at `tick`; None without points or duration.
    pub(crate) fn sample(&mut self, tick: u32) -> Option<[i32; 3]> {
        let count = self.points[0].len();
        if self.duration == 0 || count == 0 {
            return None;
        }
        if self.start.wrapping_add(self.duration) <= tick {
            return Some(self.points.each_ref().map(|axis| axis[count - 1]));
        }
        let u = f64::from(tick.wrapping_sub(self.start)) / f64::from(self.duration);
        Some(match count {
            1 => self.points.each_ref().map(|axis| axis[0]),
            2 => self.points.each_ref().map(|axis| {
                axis[0].wrapping_add((f64::from(axis[1].wrapping_sub(axis[0])) * u) as i32)
            }),
            _ => {
                let axes = self
                    .solved
                    .get_or_insert_with(|| self.points.each_ref().map(|axis| Axis::solve(axis)));
                axes.each_ref().map(|axis| axis.eval(u) as i32)
            }
        })
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn three_points_pass_through_every_knot() {
        let mut spline = CSpline::default();
        spline.add(0, 0, 0);
        spline.add(100, 50, 400);
        spline.add(200, 100, 0);
        spline.set_duration(10);
        assert_eq!(spline.sample(0), Some([0, 0, 0]));
        assert_eq!(spline.sample(5), Some([100, 50, 400]));
        assert_eq!(spline.sample(10), Some([200, 100, 0]));
        // Natural spline through a symmetric arch: the apex segment bulges.
        let quarter = spline.sample(2).unwrap();
        assert!(quarter[2] > 0 && quarter[2] < 400);
    }

    #[test]
    fn collinear_knots_stay_on_the_line() {
        let mut spline = CSpline::default();
        spline.add(0x8000, 0x8000, 0);
        spline.add(0x18000, 0x18000, 0);
        spline.add(0x28000, 0x28000, 0);
        spline.set_duration(16);
        for tick in 0..=16 {
            let [x, y, _] = spline.sample(tick).unwrap();
            assert_eq!(x, y);
            assert!((0x8000..=0x28000).contains(&x));
        }
    }

    #[test]
    fn two_points_interpolate_linearly_and_one_point_holds() {
        let mut spline = CSpline::default();
        spline.add(10, 0, 0);
        spline.set_duration(4);
        assert_eq!(spline.sample(2), Some([10, 0, 0]));
        spline.add(30, -8, 7);
        assert_eq!(spline.sample(1), Some([15, -2, 1]));
        assert_eq!(spline.sample(4), Some([30, -8, 7]));
        spline.set_duration(0);
        assert_eq!(spline.sample(0), None);
    }

    #[test]
    fn hundredth_point_is_the_last() {
        let mut spline = CSpline::default();
        for i in 0..100 {
            assert!(spline.add(i, i, i));
        }
        assert!(!spline.add(0, 0, 0));
    }
}
