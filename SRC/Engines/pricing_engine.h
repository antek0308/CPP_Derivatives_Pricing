#pragma once
#include "instrument.h"

// forward declaration <= go back here
class PricingEngine

{
public:
    virtual double calculate(const Instrument& option) const = 0;
    virtual ~PricingEngine() {}
};