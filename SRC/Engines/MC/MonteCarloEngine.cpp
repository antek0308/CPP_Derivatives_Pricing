#include "MonteCarloEngine.h"
#include "EuropeanOption.h"
#include "payoff.h"
#include <cmath>
#include <random>

// Extending the code with gatherer for MC
#include "conf_limits.h"

MonteCarloEngine::MonteCarloEngine(BlackScholesProcess process, unsigned long number_of_paths)
    : process_(process), number_of_paths_(number_of_paths)
{
}

double MonteCarloEngine::calculate(const Instrument& instrument) const
{
    // cast the generic Instrument& to EuropeanOption so we can read payoff() and expiry()
    const EuropeanOption& option = dynamic_cast<const EuropeanOption&>(instrument);
    const Payoff& payoff = option.payoff();

    std::mt19937_64 rng(12345);
    std::normal_distribution<double> norm(0.0, 1.0);

    double S0 = process_.spot();
    double T = option.expiry();

    // integrate the curves over the whole life [0,T] (for constant parameters this is r*T and vol^2*T)
    double drift     = process_.r().get_integral(0.0, T)
                     - process_.d().get_integral(0.0, T)
                     - 0.5 * process_.vol().get_integral_square(0.0, T);
    double diffusion = std::sqrt(process_.vol().get_integral_square(0.0, T));
    double discount  = std::exp(-process_.r().get_integral(0.0, T));

    ConfidenceLimits gatherer(1.96); // Setting confidence limits to 95% (1.96 = normal quantile)

    for (unsigned long i = 0; i < number_of_paths_; i++)
    {
        double Z = norm(rng);
        double ST = S0 * std::exp(drift + diffusion * Z);
        gatherer.dump_one_result(payoff(ST) * discount); // add the discounted payoff to the gatherer
    }

    std::vector<MCResult> res = gatherer.get_results_so_far();
    double price = res.back().mean;
    std_error_ = res.back().stdError; // save the error for errorEstimate()

    return price;
}

double MonteCarloEngine::errorEstimate() const
{
    return std_error_;
}
