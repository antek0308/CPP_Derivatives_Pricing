#ifndef PATH_DEPENDENT_GEOMETRIC_ASIAN_H
#define PATH_DEPENDENT_GEOMETRIC_ASIAN_H

#include "path_dependent.h"
#include "payoff_bridge.h"

// this class inherits after PathDependent class
class PathDependentGeometricAsian : public PathDependent
{
public:
    // constructor takes three arguments
    PathDependentGeometricAsian(const MJArray &look_at_times_,
                       double delivery_time_,
                       const PayOffBridge &the_payoff_);

    virtual unsigned long max_number_of_cash_flows() const;
    virtual MJArray possible_cash_flow_times() const;
    virtual unsigned long cash_flows(const MJArray &spot_values,
                                     std::vector<CashFlow> &generated_flows) const;
    virtual ~PathDependentGeometricAsian() {}
    virtual PathDependent *clone() const;

private:
    double delivery_time;
    PayOffBridge the_payoff;
    unsigned long number_of_times;
};

#endif
