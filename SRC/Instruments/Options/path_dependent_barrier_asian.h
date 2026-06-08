#ifndef PATH_DEPENDENT_BARRIER_ASIAN_H
#define PATH_DEPENDENT_BARRIER_ASIAN_H

#include "path_dependent.h"
#include "payoff_bridge.h"

// double barrier knock-out asian. basically the normal asian but if the spot
// goes below b_lower or above b_upper on any of the dates the option dies (pays 0)
class PathDependentBarrierAsian : public PathDependent
{
public:
    PathDependentBarrierAsian(const MJArray &look_at_times_,
                              double delivery_time_,
                              double b_lower_,
                              double b_upper_,
                              const PayOffBridge &the_payoff_);

    virtual unsigned long max_number_of_cash_flows() const;
    virtual MJArray possible_cash_flow_times() const;
    virtual unsigned long cash_flows(const MJArray &spot_values,
                                     std::vector<CashFlow> &generated_flows) const;
    virtual ~PathDependentBarrierAsian() {}
    virtual PathDependent *clone() const;

private:
    double delivery_time;
    double b_lower;
    double b_upper;
    PayOffBridge the_payoff;
    unsigned long number_of_times;
};

#endif
