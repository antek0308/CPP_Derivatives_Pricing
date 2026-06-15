#pragma once
#include "payoff.h"
#include <memory>
#include "instrument.h"

// THe class already genralizes to double barriers. If user want single barrier then just enters one barrier
// and the other is inf. If user want e.g. up-and-in call then chooses "In" Barrier and out is inf.

enum class Knock {In, Out};


class BarrierOption: public Instrument
{
    private:
        double expiry_;
        std::shared_ptr<Payoff> payoff_;
        double lower_barrier_;
        double upper_barrier_;
        Knock barrier_type_;
    public:
        BarrierOption(std::shared_ptr<Payoff> payoff, double expiry,
                      double lower_barrier_, double upper_barrier_,
                      Knock barrier_type_);

        
        // here are named makers to make pricing single barrier more convinient and have one class for single and
        // double barriers. If it was not implemented one would need to rember to pass inf or -inf for upper or lower barrier
        // to make pricing correct, which was not elegant
        static BarrierOption downOut (std::shared_ptr<Payoff> payoff, double expiry, double barrier);
        static BarrierOption upOut (std::shared_ptr<Payoff> payoff, double expiry, double barrier);
        static BarrierOption doubleOut (std::shared_ptr<Payoff> payoff, double expiry, double lower, double upper);
        static BarrierOption downIn (std::shared_ptr<Payoff> payoff, double expiry, double barrier);
        static BarrierOption upIn (std::shared_ptr<Payoff> payoff, double expiry, double barrier);
        static BarrierOption doubleIn (std::shared_ptr<Payoff> payoff, double expiry, double lower, double upper);

        double expiry() const;
        double lower_barrier() const;
        double upper_barrier() const;
        Knock barrier_type() const; // return Knocj type (In/Out)
        const Payoff& payoff() const;
};