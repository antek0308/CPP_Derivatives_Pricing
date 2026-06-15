#include "BrownianBridge.h"
#include <cmath>

double BrownianBridge::step_survival(
    double S_prev, double S_curr, double lower,
    double upper, double variance
) const
{
    // if S is already past the barrier at this grid point, it is knocked out
    if (S_curr <= lower || S_curr >= upper)
        return 0.0;

    // probability the path crossed the barrier between S_prev and S_curr (brownian bridge).
    // variance is the step variance (integral of vol^2).
    // if a barrier is inf (upper) or non-positive (lower) that side does not exist, so p = 0
    double p_up  = std::isfinite(upper)
                 ? std::exp(-2 * std::log(upper / S_prev) * std::log(upper / S_curr) / variance)
                 : 0.0;
    double p_low = (lower > 0.0)
                 ? std::exp(-2 * std::log(S_prev / lower) * std::log(S_curr / lower) / variance)
                 : 0.0;

    // clamp to [0,1]. The formula can go outside this range when the path starts on the
    // dead side; a value outside [0,1] just means it surely crossed, so treat as knocked out
    double s = (1.0 - p_up) * (1.0 - p_low);
    if (s < 0.0) s = 0.0;
    if (s > 1.0) s = 1.0;
    return s;
}
