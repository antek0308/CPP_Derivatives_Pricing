#pragma once

// Base class for barrier monitoring. step_survival returns the probability that the
// option survives one step. Discrete monitor returns 0 or 1, Brownian bridge returns
// a probability in between (it accounts for a possible breach inside the step).

class BarrierMonitor
{
    public:
        virtual double step_survival(
            double S_prev, double S_curr, double lower,
            double upper, double variance     // step variance (integral of vol^2)
        ) const = 0;
        // destructor
        virtual ~BarrierMonitor() {}
};