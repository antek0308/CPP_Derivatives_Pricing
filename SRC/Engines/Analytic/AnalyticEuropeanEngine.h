#pragma once
#include "pricing_engine.h"          // the base class (we inherit → full include)
#include "black_scholes_process.h"   // we hold a process by value → full include


class AnalyticEuropeanEngine: public PricingEngine
// Analytic Euriopean inherits Priicng Enigne methods
{
    private:
        BlackScholesProcess process_;
    public:
        AnalyticEuropeanEngine(BlackScholesProcess process); // give the engine its market
        double calculate(const Instrument& instrument) const override;
};