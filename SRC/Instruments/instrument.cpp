#include "instrument.h"
#include "pricing_engine.h"

void Instrument::setPricingEngine(std::shared_ptr<PricingEngine> engine)
{
    engine_ = engine;
}

double Instrument::NPV() const 
{
    return engine_ -> calculate(*this); // this Instrument
}