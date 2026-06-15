#pragma once
#include "pricing_engine.h"
#include "black_scholes_process.h"
#include "RngBase.h"
#include "BarrierMonitor.h"
#include <memory>

// Separate MonteCarlo ENgine for path-dependant payoffs.
// Barrier, touch options etc. need to keep tract of the path during whole payoff.

class MonteCarloBarrierEngine: public PricingEngine
{
    private:
        BlackScholesProcess process_;
        mutable double std_error_; // mutable because calculate() is const but I still want to save the error here
        unsigned long number_of_steps_;
        unsigned long number_of_simulations_;
        std::shared_ptr<RngBase> rng_;   // the RNG used for the simulation, passed in by the caller
        std::shared_ptr<BarrierMonitor> monitor_; // barrier monitor, passed in (defaults to Discrete if none given)
    public:
        MonteCarloBarrierEngine(BlackScholesProcess process, unsigned long number_of_steps,
                                unsigned long number_of_simulations,
                                std::shared_ptr<RngBase> rng,
                                std::shared_ptr<BarrierMonitor> monitor = nullptr
                            );
        double calculate(const Instrument& instrument) const override;
        double errorEstimate() const;
};
