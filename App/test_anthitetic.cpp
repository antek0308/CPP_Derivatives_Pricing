#include <iostream>
#include <iomanip>
#include <memory>
#include "payoff.h"
#include "BarrierOption.h"
#include "EuropeanOption.h"
#include "black_scholes_process.h"
#include "MonteCarloBarrierEngine.h"
#include "AnalyticEuropeanEngine.h"
#include "Mt19937.h"
#include "AntiThetic.h"

int main()
{
    // sample contract params from assignment
    const double expiry = 0.5;
    const double strike = 110.0;
    const double barrier = 100.0;

    auto payoff = std::make_shared<PlainVanillaPayoff>(OptionType::Call, strike);
    BlackScholesProcess process(110.0, 0.23, 0.05, 0.0); // spot, vol, r, d

    const unsigned long num_steps = 100;
    const unsigned long num_paths = 200000;

    BarrierOption down_out = BarrierOption::downOut(payoff, expiry, barrier);

    // Mt19937 WITHOUT antithetic
    auto plain_eng = std::make_shared<MonteCarloBarrierEngine>(
        process, num_steps, num_paths, std::make_shared<Mt19937Rng>(12345));
    down_out.setPricingEngine(plain_eng);
    const double out_price = down_out.NPV();
    const double out_error = plain_eng->errorEstimate();

    // Mt19937 WITH antithetic (decorator wraps it)
    auto anti_eng = std::make_shared<MonteCarloBarrierEngine>(
        process, num_steps, num_paths,
        std::make_shared<AntiThetic>(std::make_shared<Mt19937Rng>(12345)));
    down_out.setPricingEngine(anti_eng);
    const double out_price_anti = down_out.NPV();
    const double out_error_anti = anti_eng->errorEstimate();

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Down-and-out WITHOUT antithetic = " << out_price << " (MC std err " << out_error << ")\n";
    std::cout << "Down-and-out WITH antithetic = " << out_price_anti << " (MC std err " << out_error_anti << ")\n";

    return 0;
    return 0;
}
