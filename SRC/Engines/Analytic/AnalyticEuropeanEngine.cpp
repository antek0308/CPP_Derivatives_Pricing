#include "AnalyticEuropeanEngine.h"
#include "EuropeanOption.h"   // we read its payoff()/expiry()
#include "payoff.h"           // PlainVanillaPayoff: strike(), type()
#include "BlackScholes.h" // bs_price

// constructor: the engine remembers the market it prices against
AnalyticEuropeanEngine::AnalyticEuropeanEngine(BlackScholesProcess process)
    : process_(process)
{
}

double AnalyticEuropeanEngine::calculate(const Instrument& instrument) const
{
    // We are handed a GENERIC Instrument&. Through the base type only Instrument's
    // methods are visible (NPV, setPricingEngine) -- NOT payoff()/expiry().
    // The cast re-views it as a EuropeanOption, unlocking those fields.
    // Yes: without this cast, option.payoff() below would not even compile.

    // Q: so every instrument needs casting now ebecause everuy isntrument is 
    // implemented through base class?
    // Yes — but the casting lives in the engine, not the instrument.
    const EuropeanOption& option = dynamic_cast<const EuropeanOption&>(instrument);

    // payoff() returns a generic Payoff& (only operator() visible). Cast to the
    // concrete PlainVanillaPayoff to unlock strike() and type().
    const PlainVanillaPayoff& payoff =
        dynamic_cast<const PlainVanillaPayoff&>(option.payoff());

    // bs_price wants an int flag (1 = call, 0 = put); translate the enum.
    int call = (payoff.type() == OptionType::Call) ? 1 : 0;

    // Combine MARKET (process_) + CONTRACT (payoff, option) -> formula -> price.
    return bs_price(process_.spot(), payoff.strike(),
                    process_.r(), process_.d(), process_.vol(),
                    option.expiry(), call);
}
