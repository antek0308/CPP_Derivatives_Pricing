#pragma once

// This class implements brownian bridge, which replaced discrete monitoring with
// bolean "breached" with survival probability, which answers the question of 
// "what if the breach would have happened between this short dt?"

class BarrierMonitor
{
    public:
        virtual double step_survival(
            double S_prev, double S_curr, double lower,
            double upper, double vol, double dt
        ) const = 0;
        // destructor
        virtual ~BarrierMonitor() {}
};