#include "monte_carlo_engine.h"
#include "EuropeanOption.h"
#include "payoff.h"
#include <cmath>
#include <random>

MonteCarloEngine::MonteCarloEngine(BlackScholesProcess process, unsigned long number_of_paths)
    : process_(process), number_of_paths_(number_of_paths)
{
}

double MonteCarloEngine::calculate(const Instrument& instrument) const 
{
    // Dynamic casts of base classes. Again what in code should tell me that I need to do it?
    // Q: ...
    // "What in code tells me I need to cast?"
    // cast only when the method you want isn't visible on the type you currently hold.
    // instrument is Instrument& → you want payoff()/expiry(), which live on EuropeanOption, 
    // not Instrument → cast needed.
    // option.payoff() is Payoff& → you want operator(), which IS on Payoff → no cast
    //
    const EuropeanOption& option = dynamic_cast<const EuropeanOption&>(instrument);
    const Payoff& payoff = option.payoff();

    std::mt19937_64 rng(12345);
    std::normal_distribution<double> norm(0.0, 1.0);

    double vol = process_.vol();
    double S0 = process_.spot();
    double r = process_.r();
    double d = process_.d();
    double T = option.expiry();    
    double drift = (r - d - 0.5*vol*vol)*T;
    double diffusion = vol * std::sqrt(option.expiry());
    
    double sum = 0.0;

    for (int i = 0; i < number_of_paths_; i++)
    {
        double Z = norm(rng);
        double ST = S0* std::exp(drift + diffusion*Z);
        sum += payoff(ST);
    };
    double mean = sum / number_of_paths_;
    double price = mean * std::exp(-r*T);

    return price;
}