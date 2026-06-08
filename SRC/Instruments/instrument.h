#pragma once
#include <memory>

// Abstract class for instruments. This will be useful as with this design we can put any instrument
// into pricing engine. Without it, the engone would be too narrow. The engine with e.g.
// GBM process could not be attached too multiple instruments or at least it would not be elegant

//  EuropeanOption gets Instrument's methods because of inheritance, not 
//  because of abstractness. Abstract only answers a different question: 
//  "can someone write Instrument x; directly?"

// Abstract when child needs to implement it 

class PricingEngine; // forward declaration, because PRicing Engine was not yet finished at that time

class Instrument
{
    public:
        void setPricingEngine(std::shared_ptr<PricingEngine> engine);
        double NPV() const;
        virtual ~Instrument() {}

    protected:
        // private should be protected. Your engine member is private, 
        // which locks out even derived classes. EuropeanOption (a child) 
        // needs to reach it. protected = "children yes, outsiders no."
        std::shared_ptr<PricingEngine> engine_;
};