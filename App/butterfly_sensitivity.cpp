// Butterfly sensitivity in c++, whcih could be comapred with Excel

#include <iostream>
#include <iomanip>
#include <memory>
#include "payoff.h"
#include "BarrierOption.h"
#include "black_scholes_process.h"
#include "MonteCarloBarrierEngine.h"
#include "Mt19937.h"
#include <fstream>
#include "Portfolio.h"
#include "AntiThetic.h"


// Helper for pricing butterfly only
double priceButterfly(double spot, double vol, double barrier, double expiry, unsigned long num_paths)
{
    const double strike1 = 100.0; // lower strike
    const double strike2 = 110.0; // middle strike
    const double strike3 = 120.0; // upper strike

    const double r = 0.05, d = 0.0;   // expiry is now a parameter
    const unsigned long num_steps = 200;

    // Defining the process (spot, vol, r, d)
    // It is GBM, but here referenced in "finance terms"
    BlackScholesProcess process(spot, vol, r, d);

    // European PlainVanilla Payoff
    auto payoff1 = std::make_shared<PlainVanillaPayoff>(OptionType::Put, strike1);
    auto payoff2 = std::make_shared<PlainVanillaPayoff>(OptionType::Put, strike2);
    auto payoff3 = std::make_shared<PlainVanillaPayoff>(OptionType::Put, strike3);

    // Defining the isntruments - puts are out-and-out
    auto firstLeg = std::make_shared<BarrierOption>(BarrierOption::upOut(payoff1, expiry, barrier));
    auto secondLeg = std::make_shared<BarrierOption>(BarrierOption::upOut(payoff2, expiry, barrier));
    auto thirdLeg = std::make_shared<BarrierOption>(BarrierOption::upOut(payoff3, expiry, barrier));


    // Mt19937 engine with antithetic varites trick
    auto plain_eng = std::make_shared<MonteCarloBarrierEngine>(
        process, num_steps, num_paths, 
        std::make_shared<AntiThetic>(std::make_shared<Mt19937Rng>(12345)));

    firstLeg->setPricingEngine(plain_eng);
    secondLeg->setPricingEngine(plain_eng);
    thirdLeg->setPricingEngine(plain_eng);

    // Instatiating the portfolio and adding the legs
    Portfolio LongPutButterfly;
    LongPutButterfly.add(1, firstLeg);
    LongPutButterfly.add(-2, secondLeg);
    LongPutButterfly.add(1, thirdLeg);

    return LongPutButterfly.NPV();
}


int main()
{
    // fixed parameters
    const double S0 = 110.0, vol = 0.23, T0 = 0.5;
    const double barrier = 130.0; // barrier above the structure
    const unsigned long num_paths = 50000;

    // price vs spot
    std::ofstream f_spot("study_spot.csv");
    f_spot << "spot,price\n";
    for (double s = 80.0; s <= 140.0; s += 1.0)
        f_spot << s << "," << priceButterfly(s, vol, barrier, T0, num_paths) << "\n";

    // price vs vol
    std::ofstream f_vol("study_vol.csv");
    f_vol << "vol,price\n";
    for (double v = 0.01; v <= 0.80; v += 0.01)
        f_vol << v << "," << priceButterfly(S0, v, barrier, T0, num_paths) << "\n";

    // price vs barrier
    std::ofstream f_barrier("study_barrier.csv");
    f_barrier << "barrier,price\n";
    for (double b = 90.0; b <= 150.0; b += 1.0)
        f_barrier << b << "," << priceButterfly(S0, vol, b, T0, num_paths) << "\n";

    // price vs maturity
    std::ofstream f_mat("study_maturity.csv");
    f_mat << "maturity,price\n";
    for (double T = 0.05; T <= 2.0; T += 0.05)
        f_mat << T << "," << priceButterfly(S0, vol, barrier, T, num_paths) << "\n";

    std::cout << "Produced: study_spot.csv, study_vol.csv, study_barrier.csv, study_maturity.csv\n";
    return 0;
}