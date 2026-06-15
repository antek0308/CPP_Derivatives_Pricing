#include "AnalyticEuropeanEngine.h"
#include "EuropeanOption.h"   // we read its payoff()/expiry()
#include "payoff.h"           // PlainVanillaPayoff: strike(), type()
#include "BlackScholes.h" // bs_price
#include <cmath>          // sqrt

// constructor, stores the market (process)
AnalyticEuropeanEngine::AnalyticEuropeanEngine(BlackScholesProcess process)
    : process_(process)
{
}

double AnalyticEuropeanEngine::calculate(const Instrument& instrument) const
{
    // we get a generic Instrument&, which only shows Instrument methods.
    // cast it to EuropeanOption so we can use payoff() and expiry()
    const EuropeanOption& option = dynamic_cast<const EuropeanOption&>(instrument);

    // same idea: cast Payoff& to PlainVanillaPayoff to use strike() and type()
    const PlainVanillaPayoff& payoff =
        dynamic_cast<const PlainVanillaPayoff&>(option.payoff());

    // bs_price wants 1 for a call and 0 for a put
    int call = (payoff.type() == OptionType::Call) ? 1 : 0;

    double T = option.expiry();
    // Black-Scholes needs single numbers, so I take the average rate and the rms vol
    // over [0,T] from the curves (for constant parameters these are just r, d and vol)
    double r   = process_.r().get_mean(0.0, T);
    double d   = process_.d().get_mean(0.0, T);
    double vol = std::sqrt(process_.vol().get_root_mean_square(0.0, T));

    // price it with the Black-Scholes formula
    return bs_price(process_.spot(), payoff.strike(),
                    r, d, vol, T, call);
}
