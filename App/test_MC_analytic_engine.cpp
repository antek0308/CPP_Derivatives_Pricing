#include <iostream>
#include <memory>
#include "payoff.h"
#include "EuropeanOption.h"
#include "black_scholes_process.h"
#include "AnalyticEuropeanEngine.h"
#include "MonteCarloEngine.h"

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
    // Keep a typed handle: errorEstimate() lives on MonteCarloEngine, NOT on the
    // generic PricingEngine base, so a base pointer couldn't reach it.
    auto mc = std::make_shared<MonteCarloEngine>(process, num_paths);
    option.setPricingEngine(mc);
    std::cout << "MC call NPV  = " << option.NPV() << "\n";          // expect ~8.4819
    std::cout << "MC std error = " << mc->errorEstimate() << "\n";   // 95% CI half-width = 1.96*this

    return 0;
}
