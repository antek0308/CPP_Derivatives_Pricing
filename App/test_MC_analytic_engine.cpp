// Testing the analytic vs MC results
#include <iostream>
#include <memory>
#include "payoff.h"
#include "EuropeanOption.h"
#include "black_scholes_process.h"
#include "AnalyticEuropeanEngine.h"
#include "MonteCarloEngine.h"

int main()
{
    // payoff
    auto payoff = std::make_shared<PlainVanillaPayoff>(OptionType::Call, 110);

    // option = payoff + expiry
    EuropeanOption option(payoff, 0.5);

    // process (spot, vol, r, d)
    BlackScholesProcess process(110, 0.23, 0.05, 0.0);

    // Enigne, then plugged it into the option
    option.setPricingEngine(std::make_shared<AnalyticEuropeanEngine>(process));

    std::cout << "Analytic call NPV = " << option.NPV() << "\n";

    unsigned long num_paths = 1000000;
    auto mc = std::make_shared<MonteCarloEngine>(process, num_paths);
    option.setPricingEngine(mc);
    std::cout << "MC call NPV  = " << option.NPV() << "\n";
    std::cout << "MC std error = " << mc->errorEstimate() << "\n"; // 95% CI half-width = 1.96*this

    return 0;
}
