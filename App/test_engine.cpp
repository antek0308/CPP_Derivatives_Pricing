#include <iostream>
#include <memory>
#include "payoff.h"
#include "EuropeanOption.h"
#include "black_scholes_process.h"
#include "AnalyticEuropeanEngine.h"
#include "monte_carlo_engine.h"

int main()
{
    // 1. payoff (what kind of option)
    auto payoff = std::make_shared<PlainVanillaPayoff>(OptionType::Call, 110);

    // 2. the option = payoff + expiry
    EuropeanOption option(payoff, 0.5);

    // 3. the market, bundled as a process   (spot, vol, r, d)
    BlackScholesProcess process(110, 0.23, 0.05, 0.0);

    // 4 + 5. build the engine around the market, then plug it into the option
    option.setPricingEngine(std::make_shared<AnalyticEuropeanEngine>(process));

    std::cout << "Analytic call NPV = " << option.NPV() << "\n";   // expect 8.4819

    unsigned long num_paths = 1000000;
    option.setPricingEngine(std::make_shared<MonteCarloEngine>(process, num_paths));
    std::cout << "MC call NPV = " << option.NPV() << "\n";   // expect 8.4819

    return 0;
}
