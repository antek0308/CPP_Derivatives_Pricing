// File for testing the Portfolio class

#include <iostream>
#include <iomanip>
#include <memory>
#include "payoff.h"
#include "BarrierOption.h"
#include "black_scholes_process.h"
#include "MonteCarloBarrierEngine.h"
#include "Mt19937.h"
// #include "AntiThetic.h"
#include "Portfolio.h"

int main()
{
    // Numbers from assignment
    const double S0 = 110, vol = 0.23, r = 0.05, d = 0.0, expiry = 0.5;

    // Strikes and barrier not provided
    // As it is long fly, it makes sense to position the middle strike at S0
    const double strike1 = 100.0; // lower strike
    const double strike2 = 110.0; // middle strike
    const double strike3 = 120.0; // upper strike
    const double barrier = 130.0; // barrier "above" whole structure.

    // MC simulation steps and number of simulations not provided 
    const unsigned long num_steps = 100;
    const unsigned long num_paths = 100000;
    
    // Defining the process (spot, vol, r, d)
    // It is GBM, but here referenced in "finance terms"
    BlackScholesProcess process(S0, vol, r, d);

    // European PlainVanilla Payoff
    auto payoff1 = std::make_shared<PlainVanillaPayoff>(OptionType::Put, strike1);
    auto payoff2 = std::make_shared<PlainVanillaPayoff>(OptionType::Put, strike2);
    auto payoff3 = std::make_shared<PlainVanillaPayoff>(OptionType::Put, strike3);

    // Defining the isntruments - puts are out-and-out
    auto firstLeg = std::make_shared<BarrierOption>(BarrierOption::upOut(payoff1, expiry, barrier));
    auto secondLeg = std::make_shared<BarrierOption>(BarrierOption::upOut(payoff2, expiry, barrier));
    auto thirdLeg = std::make_shared<BarrierOption>(BarrierOption::upOut(payoff3, expiry, barrier));

    // Mt19937 engine without antithetic varites trick
    auto plain_eng = std::make_shared<MonteCarloBarrierEngine>(
        process, num_steps, num_paths, std::make_shared<Mt19937Rng>(12345));
    
    firstLeg->setPricingEngine(plain_eng);
    secondLeg->setPricingEngine(plain_eng);
    thirdLeg->setPricingEngine(plain_eng);

    // Instatiating the portfolio and adding the legs
    Portfolio LongPutButterfly;
    LongPutButterfly.add(1, firstLeg);
    LongPutButterfly.add(-2, secondLeg);
    LongPutButterfly.add(1, thirdLeg);
    
    const double price = LongPutButterfly.NPV();

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Price = " << price << "\n";
    
    return 0;
}
