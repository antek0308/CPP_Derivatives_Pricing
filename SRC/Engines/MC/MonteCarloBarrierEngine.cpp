#include "MonteCarloBarrierEngine.h"
#include "BarrierOption.h"
#include "payoff.h"
#include <cmath>
#include <vector>
#include "BarrierMonitor.h"
#include "DiscreteMonitor.h"

// Extending the code with gatherer for MC
#include "conf_limits.h"

MonteCarloBarrierEngine::MonteCarloBarrierEngine(BlackScholesProcess process, unsigned long number_of_steps,
                                                 unsigned long number_of_simulations,
                                                 std::shared_ptr<RngBase> rng,
                                                 std::shared_ptr<BarrierMonitor> monitor)
    : process_(process), number_of_steps_(number_of_steps), number_of_simulations_(number_of_simulations),
      rng_(rng), monitor_(monitor)
{
    // default to discrete monitoring if the caller didn't supply one
    if (!monitor_) monitor_ = std::make_shared<DiscreteMonitor>();
}

double MonteCarloBarrierEngine::calculate(const Instrument& instrument) const
{
    const BarrierOption& option = dynamic_cast<const BarrierOption&>(instrument);

    const Payoff& payoff = option.payoff();

    double S0 = process_.spot();
    double T = option.expiry();

    double dt = T / static_cast<double>(number_of_steps_);

    // precompute per-step drift / diffusion / variance from the (possibly
    // time-varying) curves. For constant params every step is identical.
    std::vector<double> drifts(number_of_steps_);
    std::vector<double> diffusions(number_of_steps_);
    std::vector<double> variances(number_of_steps_);
    for (unsigned long i = 0; i < number_of_steps_; i++)
    {
        double t1 = i * dt, t2 = (i + 1) * dt;
        double var = process_.vol().get_integral_square(t1, t2);
        drifts[i]     = process_.r().get_integral(t1, t2)
                      - process_.d().get_integral(t1, t2)
                      - 0.5 * var;
        diffusions[i] = std::sqrt(var);
        variances[i]  = var;
    }
    double discount = std::exp(-process_.r().get_integral(0.0, T));

    // Barrier-specific
    double lower_b = option.lower_barrier();
    double upper_b = option.upper_barrier();
    Knock barrier_type = option.barrier_type();

    // Setting confidence limits to 95% (1.96 = normal quantile)
    ConfidenceLimits gatherer(1.96);

    std::vector<double> variates(number_of_steps_); // one path's worth of N(0,1) draws

    for (unsigned long j = 0; j < number_of_simulations_; j++)
    {
        double S = S0;
        double payoff_value = 0.0;

        rng_->get_gaussians(variates); // fill this path's draws from the injected RNG

        // in order to calculate the brownian brdige we need to trace the previous S, so 
        // here is a trailing S_prev
        double S_prev = S0;
        double survival = 1.0;

        // born-dead / born-activated: if the spot already sits past the barrier at t=0,
        // an OUT option is knocked out (and an IN option activated) immediately.
        if (S0 <= lower_b || S0 >= upper_b) survival = 0.0;

        for (unsigned long i = 0; i < number_of_steps_; i++)
        {
            double Z = variates[i];
            S *= std::exp(drifts[i] + diffusions[i] * Z);
            survival *= monitor_->step_survival(S_prev, S, lower_b, upper_b, variances[i]);
            S_prev = S;
            // NOTE: do NOT break when survival hits 0 — a knock-IN still needs S_T at
            // expiry. (For knock-OUT, survival=0 zeroes the payoff anyway, so nothing lost.)
        }
        // Alive by default. If breached and out barruer then it is not alive
        if (barrier_type == Knock::Out) payoff_value = survival * payoff(S);
        if (barrier_type == Knock::In)  payoff_value = (1.0 - survival) * payoff(S);

        gatherer.dump_one_result(payoff_value * discount);
    }

    std::vector<MCResult> res = gatherer.get_results_so_far();
    double price = res.back().mean;
    std_error_ = res.back().stdError; // cache into the mutable member for errorEstimate()

    return price;
}

double MonteCarloBarrierEngine::errorEstimate() const
{
    return std_error_;
}
