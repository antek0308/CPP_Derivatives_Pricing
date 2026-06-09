#include <iostream>
#include <memory>
#include "payoff.h"
#include "EuropeanOption.h"
#include "black_scholes_process.h"
#include "AnalyticEuropeanEngine.h"
#include "MonteCarloEngine.h"

int main()
{
    auto payoff = std::make_shared<PlainVanillaPayoff>(OptionType::Call, 110);
    EuropeanOption option(payoff, 0.5);

    BlackScholesProcess process(110, 0.23, 0.05, 0.0);

    option.setPricingEngine(std::make_shared<AnalyticEuropeanEngine>(process));

    unsigned long num_paths = 1000000;
    auto mc = std::make_shared<MonteCarloEngine>(process, num_paths);
    option.setPricingEngine(mc);
    std::cout << "MC call NPV  = " << option.NPV() << "\n";          // expect ~8.4819
    std::cout << "MC std error = " << mc->errorEstimate() << "\n";   // 95% CI half-width = 1.96*this

    return 0;
}
