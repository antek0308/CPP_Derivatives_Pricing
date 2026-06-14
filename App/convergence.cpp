#include <iostream>
#include <iomanip>
#include <memory>
#include <fstream>
#include <utility>   // std::pair
#include "payoff.h"
#include "BarrierOption.h"
#include "black_scholes_process.h"
#include "MonteCarloBarrierEngine.h"
#include "Mt19937.h"

// Price ONE barrier option (the butterfly's middle leg: an up-and-out put,
// K = 110, barrier = 130) with `num_paths` Monte Carlo paths.
// Returns {price, std_error}.  A single option (not the Portfolio) so we have
// access to engine->errorEstimate().
std::pair<double, double> priceOneOption(unsigned long num_paths)
{
    const double S0 = 110.0, vol = 0.23, r = 0.05, d = 0.0, expiry = 0.5;
    const double strike = 110.0, barrier = 130.0;
    const unsigned long num_steps = 200;

    BlackScholesProcess process(S0, vol, r, d);

    auto payoff = std::make_shared<PlainVanillaPayoff>(OptionType::Put, strike);
    auto option = std::make_shared<BarrierOption>(BarrierOption::upOut(payoff, expiry, barrier));

    // fresh engine with THIS path count + fresh RNG seed -> each row reproducible
    auto engine = std::make_shared<MonteCarloBarrierEngine>(
        process, num_steps, num_paths, std::make_shared<Mt19937Rng>(12345));
    option->setPricingEngine(engine);

    double price = option->NPV();
    double error = engine->errorEstimate();
    return {price, error};
}

int main()
{
    std::ofstream f("convergence.csv");
    f << "num_paths,price,std_error\n";

    std::cout << std::fixed << std::setprecision(6);
    for (unsigned long n = 1024; n <= (1u << 20); n *= 2)   // 2^10 ... 2^20
    {
        auto [price, error] = priceOneOption(n);            // structured binding unpacks the pair
        f << n << "," << price << "," << error << "\n";
        std::cout << "n = " << n << "   price = " << price << "   std_err = " << error << "\n";
    }

    std::cout << "Produced: convergence.csv\n";
    return 0;
}
