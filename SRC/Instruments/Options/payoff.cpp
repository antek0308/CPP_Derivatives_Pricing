#include "option_type.h"
#include <cmath>
#include <algorithm> // for std::max
#include "payoff.h"

// COnstructor
PlainVanillaPayoff::PlainVanillaPayoff(OptionType type, double strike)
    : type_(type), strike_(strike) 
{
}

// Implementation of Put and Call Option payoffs
double PlainVanillaPayoff:: operator() (double spot_) const
{
    // Call
    if (type_ == OptionType::Call)
    {
        return std::max(spot_ - strike_, 0.0);
    }
    // Put
    else if (type_ == OptionType::Put)
    {
        return std::max(strike_ - spot_, 0.0);
    }
}

OptionType PlainVanillaPayoff::type() const {return type_; } // return option type Call or Put
double PlainVanillaPayoff::strike() const {return strike_; }

// Destructor
Payoff* PlainVanillaPayoff::clone() const
{
    return new PlainVanillaPayoff(*this);
}