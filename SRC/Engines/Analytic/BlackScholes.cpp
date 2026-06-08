#include <cmath>
#include "normals.h"
#include "BlackScholes.h"

// Here is european call option function with analytic BS
// I created it as European calls/puts  should be the base cases anyway
// and to check MC engine
double bs_price(double S, double K, double r, double d, double vol, double T, int call)
{
    // Enter 1 for a call and 0 for a put
    double d1 = (std::log(S/K) + (r - d + 0.5*vol*vol)*T) / (vol*std::sqrt(T));
    double d2 = d1 - vol*std::sqrt(T);

    double price;
    if (call == 1)
    {
        price = CumulativeNormal(d1)*S*std::exp(-d*T) - CumulativeNormal(d2)*K*std::exp(-r*T);
    }
    else
    {
        price = CumulativeNormal(-d2)*K*std::exp(-r*T) - CumulativeNormal(-d1)*S*std::exp(-d*T);
    }
    return price;
}