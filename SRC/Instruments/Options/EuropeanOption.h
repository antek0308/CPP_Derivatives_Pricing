#pragma once
#include "payoff.h"
#include <memory>
#include "instrument.h"

// ?Why that's better — it splits an option into two independent ideas, exactly like QuantLib:
// option = exercise style + payoff
// "European" describes the exercise style: you can only exercise at expiry T (vs American 
// = anytime, Bermudan = some dates).
// the payoff describes the shape: call, put, digital, …
// Because EuropeanOption holds a Payoff rather than knowing about strikes itself, the same 
// class prices a European call, a European put, a European digital, anything — just hand it 
// a different payoff. VanillaOption could never do that. That generality is the point.


// The commented version stores a raw Payoff* payoff_;. A raw pointer is a resource you must manage 
// by hand, which forces the Rule of Three: if a class manages a raw resource, you must write all 
// three of —

// destructor (~EuropeanOption() { delete payoff_; }) — or you leak memory,
// copy constructor (deep-copy via clone()) — or copies share & double-free,
// copy assignment operator — same reason.
// That's the whole mess in the comment. Miss one → crashes or leaks.

// Now your real version stores a std::shared_ptr<Payoff> payoff_;. A shared_ptr is a class that already
//  has a correct destructor, copy constructor, and assignment built into it. 

// NPV and setPricingEngine() are inhereted for free
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

// Tool	What it is	Who uses it
// Wrapper<T>	hand-rolled; value semantics (copies via clone())	Joshi's book / this codebase
// std::shared_ptr<T>	standard smart pointer; shared ownership (ref-counted)	QuantLib, everywhere
// std::unique_ptr<T>	standard smart pointer; single owner	modern C++ default


// EuropeanOption WITHOUT Wrapper — for comparison
//   class EuropeanOption {
//   public:
//       EuropeanOption(const Payoff& p, double exp)
//           : payoff_(p.clone()), expiry_(exp) {}        // clone by hand
//       ~EuropeanOption() { delete payoff_; }            // free by hand
//       EuropeanOption(const EuropeanOption& o)          // copy ctor by hand
//           : payoff_(o.payoff_->clone()), expiry_(o.expiry_) {}
//       EuropeanOption& operator=(const EuropeanOption&); // assignment by hand too...
//       const Payoff& payoff() const { return *payoff_; }
//       double expiry() const { return expiry_; }
//   private:
//       Payoff* payoff_;
//       double  expiry_;
//   };
