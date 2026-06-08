#include "black_scholes_process.h"

BlackScholesProcess::BlackScholesProcess(double spot, double vol, double r, double d)
    : spot_(spot), vol_(vol), r_(r), d_(d)
{
}

// Accessors:
double BlackScholesProcess::r() const {return r_;}
double BlackScholesProcess::d() const {return d_;}
double BlackScholesProcess::vol() const {return vol_;}
double BlackScholesProcess::spot() const {return spot_;}