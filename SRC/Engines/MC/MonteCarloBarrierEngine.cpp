#include "MonteCarloBarrierEngine.h"
#include "BarrierOption.h"
#include "payoff.h"
#include <cmath>
#include <random>

// Extending the code with gatherer for MC
#include "conf_limits.h"

MonteCarloBarrierEngine::MonteCarloBarrierEngine(BlackScholesProcess process, unsigned long number_of_steps,
                                                 unsigned long number_of_simulations)
    : process_(process), number_of_steps_(number_of_steps), number_of_simulations_(number_of_simulations)
{
}

double MonteCarloBarrierEngine::calculate(const Instrument& instrument) const
{
    const BarrierOption& option = dynamic_cast<const BarrierOption&>(instrument);

    const Payoff& payoff = option.payoff();

    std::mt19937_64 rng(12345);
    std::normal_distribution<double> norm(0.0, 1.0);

    double vol = process_.vol();
    double S0 = process_.spot();
    double r = process_.r();
    double d = process_.d();
    double T = option.expiry();

    double dt = T / static_cast<double>(number_of_steps_);
    double drift = (r - d - 0.5*vol*vol)*dt;
    double diffusion = vol * std::sqrt(dt);

    // Barrier-specific
    double lower_b = option.lower_barrier();
    double upper_b = option.upper_barrier();
    Knock barrier_type = option.barrier_type();

    // Setting confidence limits to 95% (1.96 = normal quantile)
    ConfidenceLimits gatherer(1.96);

    for (unsigned long j = 0; j < number_of_simulations_; j++)
    {
        double S = S0;
        bool breached = false;
        double payoff_value = 0.0;

        for (unsigned long i = 0; i < number_of_steps_; i++)
        {
            double Z = norm(rng);
            S *= std::exp(drift + diffusion * Z);
            if (S <= lower_b || S >= upper_b) {breached = true;};
        }
        // Alive by default. If breached and out barruer then it is not alive
        bool alive = true;
        if (barrier_type == Knock::Out && breached==true){alive=false;};
        if (barrier_type == Knock::In  && !breached) { alive = false; }

        if (alive) { payoff_value = payoff(S); }
        gatherer.dump_one_result(payoff_value * std::exp(-r * T));

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
