#pragma once
#include "pricing_engine.h"
#include "black_scholes_process.h"

class MonteCarloEngine: public PricingEngine
// Analytic Euriopean inherits Priicng Enigne methods
{
    private:
        BlackScholesProcess process_;
        unsigned long number_of_paths_;
    public:
        MonteCarloEngine(BlackScholesProcess process, unsigned long number_of_paths); // same process as in ANalytic
        double calculate(const Instrument& instrument) const override;
};