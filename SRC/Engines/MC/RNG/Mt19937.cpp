#include "Mt19937.h"
#include <random>

Mt19937Rng::Mt19937Rng(unsigned long seed)
    : rng_(seed)        // seed the generator (norm_ defaults to N(0,1))
{
}

void Mt19937Rng::get_gaussians(std::vector<double>& z)
{
    // fill the vector with N(0,1) draws
    for (double&  zi:z)
    {
        zi = norm_(rng_); // draw one N(0,1) intoeach element
    }
}