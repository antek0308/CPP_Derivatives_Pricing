#include "black_scholes_process.h"

// constant params - wraps each scalar in a ParametersConstant
BlackScholesProcess::BlackScholesProcess(double spot, double vol, double r, double d)
    : spot_(spot), vol_(ParametersConstant(vol)), r_(ParametersConstant(r)), d_(ParametersConstant(d))
{
}

// term-structure market - caller supplies the curves directly
BlackScholesProcess::BlackScholesProcess(double spot, Parameters vol, Parameters r, Parameters d)
    : spot_(spot), vol_(vol), r_(r), d_(d)
{
}

// Accessors:
double BlackScholesProcess::spot() const { return spot_; }
const Parameters& BlackScholesProcess::vol() const { return vol_; }
const Parameters& BlackScholesProcess::r() const { return r_; }
const Parameters& BlackScholesProcess::d() const { return d_; }