#pragma once
#include <memory>

// Abstract class for instruments. With such design I can put any instrument into into pricing engine. 
// Without it, the engone would be too narrow. The engine with e.g.
// GBM process could not be attached too multiple instruments or at least it would not be elegant

//  EuropeanOption gets Instrument's methods because of inheritance

class PricingEngine; // forward declaration, because PRicing Engine was not yet finished at that time

class Instrument
{
    public:
        void setPricingEngine(std::shared_ptr<PricingEngine> engine);
        virtual double NPV() const; // virtual so that when something holds this
        // as a shared_ptr<Instrument> and calls NPV(), the Portfolio's summing 
        // version runs - not the base engine version
        virtual ~Instrument() {}

    // protcted so child class can reach it
    protected:
        std::shared_ptr<PricingEngine> engine_;
};