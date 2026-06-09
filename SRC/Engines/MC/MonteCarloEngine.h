#pragma once
#include "pricing_engine.h"
#include "black_scholes_process.h"

class MonteCarloEngine: public PricingEngine
// Analytic Euriopean inherits Priicng Enigne methods
{
    private:
        BlackScholesProcess process_;
        mutable double std_error_; // calculate() is const (promises not to change the engine), but we do want to cache the error in it
        unsigned long number_of_paths_;
    public:
        MonteCarloEngine(BlackScholesProcess process, unsigned long number_of_paths); // same process as in ANalytic
        double calculate(const Instrument& instrument) const override;
        double errorEstimate() const;
};
