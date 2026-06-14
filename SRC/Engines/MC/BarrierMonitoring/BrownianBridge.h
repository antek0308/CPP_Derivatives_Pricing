#pragma once
#include "BarrierMonitor.h"

class BrownianBridge : public BarrierMonitor
{
    private:
    public:
        double step_survival(double S_prev, double S_curr, double lower,
            double upper, double variance) const override;
};