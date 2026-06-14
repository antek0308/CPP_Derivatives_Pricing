#include <iostream>
#include <iomanip>
#include <memory>
#include "payoff.h"
#include "BarrierOption.h"
#include "black_scholes_process.h"
#include "MonteCarloBarrierEngine.h"
#include "Mt19937.h"
#include "DiscreteMonitor.h"
#include "BrownianBridge.h"

// Price a down-and-out call with a given step count and a given monitor.
// Barrier sits CLOSE to spot (105 vs 110) so between-step crossings are frequent
// -> the discrete-vs-bridge gap is large and easy to see.
double priceDownOut(unsigned long num_steps, std::shared_ptr<BarrierMonitor> monitor)
{
    const double S0 = 110.0, vol = 0.23, r = 0.05, d = 0.0, expiry = 0.5;
    const double strike = 110.0, barrier = 105.0;
    const unsigned long num_paths = 100000;

    BlackScholesProcess process(S0, vol, r, d);
    auto payoff = std::make_shared<PlainVanillaPayoff>(OptionType::Call, strike);
    auto option = std::make_shared<BarrierOption>(BarrierOption::downOut(payoff, expiry, barrier));

    // same seed for both monitors -> the difference is purely the monitoring scheme
    auto engine = std::make_shared<MonteCarloBarrierEngine>(
        process, num_steps, num_paths, std::make_shared<Mt19937Rng>(12345), monitor);
    option->setPricingEngine(engine);
    return option->NPV();
}

int main()
{
    std::cout << std::fixed << std::setprecision(5);
    std::cout << "steps |  discrete  |   bridge   |   diff\n";
    std::cout << "------+------------+------------+---------\n";

    unsigned long steps[] = {5, 10, 25, 50, 100, 200, 500};
    for (unsigned long n : steps)
    {
        double disc   = priceDownOut(n, std::make_shared<DiscreteMonitor>());
        double bridge = priceDownOut(n, std::make_shared<BrownianBridge>());
        std::cout << std::setw(5) << n
                  << " | " << std::setw(10) << disc
                  << " | " << std::setw(10) << bridge
                  << " | " << std::setw(8) << (disc - bridge) << "\n";
    }
    return 0;
}
