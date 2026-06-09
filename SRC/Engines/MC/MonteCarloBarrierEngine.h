#pragma once
#include "pricing_engine.h"
#include "black_scholes_process.h"
#include "RngBase.h"
#include <memory>

// Separate MonteCarlo ENgine for path-dependant payoffs.
// Barrier, touch options etc. need to keep tract of the path during whole payoff.

class MonteCarloBarrierEngine: public PricingEngine
{
    private:
        BlackScholesProcess process_;
        mutable double std_error_; // calculate() is const (promises not to change the engine), but we do want to cache the error in it
        unsigned long number_of_steps_;
        unsigned long number_of_simulations_;
        std::shared_ptr<RngBase> rng_;   // the engine HOLDS its RNG (injected by the caller)
    public:
        MonteCarloBarrierEngine(BlackScholesProcess process, unsigned long number_of_steps,
                                unsigned long number_of_simulations,
                                std::shared_ptr<RngBase> rng);
        double calculate(const Instrument& instrument) const override;
        double errorEstimate() const;
};
