#include "Portfolio.h"
#include <vector>
#include <memory>
#include <utility>


void Portfolio::add(double weight, std::shared_ptr<Instrument> leg)
{
    legs_.push_back({weight, leg});
}

double Portfolio::NPV() const
{
    double sum = 0.0;
    // Lopping through portfolio and summing the weight * portfolio value
    for (const auto& leg : legs_)
    {
        double NPV_single = leg.second -> NPV();
        double weight = leg.first;
        sum += weight*NPV_single;

    };
    return sum;
}
