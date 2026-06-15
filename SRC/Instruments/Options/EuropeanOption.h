#pragma once
#include "payoff.h"
#include <memory>
#include "instrument.h"

// This implementation splits an option into two independent ideas, exactly like QuantLib:
// option = exercise style + payoff
// "European" describes the exercise style and the payoff describes the shape: call, put, digital, etc.
// Because EuropeanOption holds a Payoff it can price an European call/put/digital

// std::shared_ptr is used as it already has a correct destructor,
// copy constructor and assignment. I wanted to omit "Wrapper" to form a habit of using standard solutions,
// even though Wrapper was certainly more convenient, but I had problem remebering what I need to put in the classes
// when I did not have ready "Wrapper", so I just acted as if I did not have it.

// NPV and setPricingEngine() are inherted for free
class EuropeanOption: public Instrument
{
    private:
        double expiry_;
        std::shared_ptr<Payoff> payoff_;

    public:
        EuropeanOption(std::shared_ptr<Payoff> payoff, double expiry);
        double expiry() const;
        const Payoff& payoff() const;
};