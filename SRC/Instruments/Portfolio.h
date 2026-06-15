#pragma once
#include "instrument.h"
#include <vector>
#include <memory>
#include <utility>

// Implementation of Portfolio, which is alternative to COnstrcuting fly payoff.
// I prefer to have it as such, becuase if one decided to have e.g. all different structures
// then the whole repo would get messy and not readble. NMomrevoer I want to keep options\
// coming into portfolio "independant". Technically with fly payoff I could run one MC,
// but I think that neater is without it.

class Portfolio : public Instrument
{
    private:
        // now one needs to add as e.g.: XYZ.add(-2, SomeLeg)
        std::vector<std::pair<double, std::shared_ptr<Instrument>>> legs_;
    public:
        // Adding the leg to the whatever position
        void add(double weight, std::shared_ptr<Instrument> leg);
        // Return the NPV of whole position
        virtual double NPV() const override;
};