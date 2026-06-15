#pragma once
#include "option_type.h"


// abstract base interface every payoff uses
// For the time being just Put and Call
class Payoff
{
    public:
        virtual double operator()(double spot) const = 0;
        virtual Payoff* clone() const = 0; // polymorphic copy
        virtual ~Payoff() {}
};

class PlainVanillaPayoff : public Payoff
{
    private:
        OptionType type_; // type from option_type i.e. OptionType::Call or OptionType::Put
        double strike_;
    public:
        // Initilization
        PlainVanillaPayoff(OptionType type, double strike);

        // Functor for spot
        double operator()(double spot) const override;
        OptionType type() const;
        double strike() const;
        Payoff* clone() const override;
};