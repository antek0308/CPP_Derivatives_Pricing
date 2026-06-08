#include "EuropeanOption.h"
#include <memory>

EuropeanOption::EuropeanOption(std::shared_ptr<Payoff> payoff, double expiry)
    : expiry_(expiry), payoff_(payoff)
{
}

double EuropeanOption::expiry() const { return expiry_; }
const Payoff& EuropeanOption::payoff() const { return *payoff_; } // *becuase this is a pointer