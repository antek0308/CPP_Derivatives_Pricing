#include "BrownianBridge.h"
#include <cmath>

double BrownianBridge::step_survival(
    double S_prev, double S_curr, double lower,
    double upper, double variance
) const
{
    // hard hit at the grid point -> definitely knocked out
    if (S_curr <= lower || S_curr >= upper)
        return 0.0;

    // probability the bridge crossed each barrier between S_prev and S_curr.
    // `variance` is the increment variance for this step (= integral of vol^2).
    // guards: +inf upper / non-positive lower => that side is absent (p = 0).
    double p_up  = std::isfinite(upper)
                 ? std::exp(-2 * std::log(upper / S_prev) * std::log(upper / S_curr) / variance)
                 : 0.0;
    double p_low = (lower > 0.0)
                 ? std::exp(-2 * std::log(S_prev / lower) * std::log(S_curr / lower) / variance)
                 : 0.0;

    // clamp to a valid probability: the formula can over/undershoot when a path
    // straddles the barrier (e.g. it started on the dead side). A survival outside
    // [0,1] means the path definitely crossed -> treat as a certain knock-out.
    double s = (1.0 - p_up) * (1.0 - p_low);
    if (s < 0.0) s = 0.0;
    if (s > 1.0) s = 1.0;
    return s;
}
