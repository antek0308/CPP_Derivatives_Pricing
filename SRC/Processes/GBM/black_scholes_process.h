#pragma once
#include "parameters.h"

// Data Collector for BS Process.
// vol / r / d are Parameters (curves): a flat ParametersConstant for the
// constant case, any term-structure subclass later. spot stays a scalar.
class BlackScholesProcess
{
    private:
        double spot_;
        Parameters vol_;
        Parameters r_;
        Parameters d_;
    public:
        BlackScholesProcess(double spot, double vol, double r, double d);
        // passes curves directly (term structure)
        BlackScholesProcess(double spot, Parameters vol, Parameters r, Parameters d);

        double spot() const;
        const Parameters& vol() const;
        const Parameters& r() const;
        const Parameters& d() const;
};
