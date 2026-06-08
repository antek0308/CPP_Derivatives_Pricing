#pragma once
#include "option_type.h"


// abstract base interface every payoff uses
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
        OptionType type_;
        double strike_;
    public:
        PlainVanillaPayoff(OptionType type, double strike);

        double operator()(double spot) const override;
        OptionType type() const;
        double strike() const;
        Payoff* clone() const override;

};