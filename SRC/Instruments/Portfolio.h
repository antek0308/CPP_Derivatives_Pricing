#pragma once
#include "instrument.h"
#include <vector>
#include <memory>
#include <utility>

class Portfolio : public Instrument
{
    //
    private:
        std::vector<std::pair<double, std::shared_ptr<Instrument>>> legs_;
    public:
        // Adding the leg to the whatever position
        void add(double weight, std::shared_ptr<Instrument> leg);
        // Return the NPV of whole position
        virtual double NPV() const override;
};