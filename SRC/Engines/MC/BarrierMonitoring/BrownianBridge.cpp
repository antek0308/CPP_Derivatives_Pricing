#include "BrownianBridge.h"
#include <cmath>

double BrownianBridge::step_survival(
    double S_prev, double S_curr, double lower,
    double upper, double vol, double dt
) const
{
    // hard hit at the grid point -> definitely knocked out
    if (S_curr <= lower || S_curr >= upper)
        return 0.0;

    // probability the bridge crossed each barrier between S_prev and S_curr.
    // guards: +inf upper / non-positive lower => that side is absent (p = 0).
    double p_up  = std::isfinite(upper)
                 ? std::exp(-2 * std::log(upper / S_prev) * std::log(upper / S_curr) / (vol * vol * dt))
                 : 0.0;
    double p_low = (lower > 0.0)
                 ? std::exp(-2 * std::log(S_prev / lower) * std::log(S_curr / lower) / (vol * vol * dt))
                 : 0.0;

    return (1.0 - p_up) * (1.0 - p_low);
}
