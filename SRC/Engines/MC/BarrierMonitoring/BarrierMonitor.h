#pragma once

// This class implements brownian bridge, which replaced discrete monitoring with
// bolean "breached" with survival probability, which answers the question of 
// "what if the breach would have happened between this short dt?"

class BarrierMonitor
{
    public:
        virtual double step_survival(
            double S_prev, double S_curr, double lower,
            double upper, double variance     // increment variance for this step (= integral of vol^2)
        ) const = 0;
        // destructor
        virtual ~BarrierMonitor() {}
};