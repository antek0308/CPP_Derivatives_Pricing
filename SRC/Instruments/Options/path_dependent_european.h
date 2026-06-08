#ifndef PATH_DEPENDENT_EUROPEAN_H
#define PATH_DEPENDENT_EUROPEAN_H

#include "path_dependent.h"
#include "payoff_bridge.h"

// this class inherits after PathDependent class
class PathDependentEuropean : public PathDependent
{
public:
    // constructor takes three arguments
    PathDependentEuropean(const MJArray &look_at_times_,
                       double delivery_time_,
                       const PayOffBridge &the_payoff_);

    virtual unsigned long max_number_of_cash_flows() const;
    virtual MJArray possible_cash_flow_times() const;
    virtual unsigned long cash_flows(const MJArray &spot_values,
                                     std::vector<CashFlow> &generated_flows) const;
    virtual ~PathDependentEuropean() {}
    virtual PathDependent *clone() const;

private:
    double delivery_time;
    PayOffBridge the_payoff;
    unsigned long number_of_times;
};

#endif
