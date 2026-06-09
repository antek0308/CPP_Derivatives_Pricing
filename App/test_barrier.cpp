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

    const unsigned long num_steps = 100; // steps per path
    const unsigned long num_paths = 200000; // Monte Carlo samples

    // Vanilla reference - analytic European call
    EuropeanOption vanilla(payoff, expiry);
    vanilla.setPricingEngine(std::make_shared<AnalyticEuropeanEngine>(process));
    const double vanilla_price = vanilla.NPV();

    // ---- the two barrier halves ---
    // Each leg gets its OWN engine with a fresh RNG seeded the same (12345), so
    // both see identical paths => the parity holds path-by-path and the gap is
    // pure Monte Carlo noise.
    auto out_engine = std::make_shared<MonteCarloBarrierEngine>
    (
        process, num_steps, num_paths, std::make_shared<Mt19937Rng>(12345)
    );

    BarrierOption down_out = BarrierOption::downOut(payoff, expiry, barrier);
    down_out.setPricingEngine(out_engine);
    const double out_price = down_out.NPV();
    const double out_error = out_engine->errorEstimate();

    auto in_engine = std::make_shared<MonteCarloBarrierEngine>
    (
        process, num_steps, num_paths, std::make_shared<Mt19937Rng>(12345)
    );
    BarrierOption down_in = BarrierOption::downIn(payoff, expiry, barrier);
    down_in.setPricingEngine(in_engine);
    const double in_price = down_in.NPV();

    // ---- in/out parity:  downOut + downIn  ==  vanilla ----
    // On every path the option either touches the barrier (In pays, Out pays 0)
    // or it doesn't (Out pays, In pays 0); the sum is always the vanilla payoff.
    const double parity_sum = out_price + in_price;
    const double parity_gap = parity_sum - vanilla_price;

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Vanilla call (analytic) = " << vanilla_price << "\n";
    std::cout << "Down-and-out call = " << out_price << " (MC std err " << out_error << ")\n";
    std::cout << "Down-and-in call = " << in_price  << "\n";
    std::cout << "Out + In = " << parity_sum << "\n";
    std::cout << "Parity gap vs vanilla = " << parity_gap;

    return 0;
}
