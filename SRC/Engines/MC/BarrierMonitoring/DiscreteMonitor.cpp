#include "DiscreteMonitor.h"

double DiscreteMonitor::step_survival(
    double S_prev, double S_curr, double lower,
    double upper, double vol, double dt
) const
{
    // 0 if knocked out and 1 if not
    double breached = 1.0; 

    if (S_curr <= lower || S_curr >= upper)
    {
        breached = 0.0;
    }
    return breached;
}