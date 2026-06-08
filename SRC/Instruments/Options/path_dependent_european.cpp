#include "path_dependent_european.h"
#include <cmath>

PathDependentEuropean::PathDependentEuropean(const MJArray &look_at_times_,
                                       double delivery_time_,
                                       const PayOffBridge &the_payoff_)
    : PathDependent(look_at_times_),
      delivery_time(delivery_time_),
      the_payoff(the_payoff_),
      number_of_times(look_at_times_.size())
{
}

// definitions of methods
unsigned long PathDependentEuropean::max_number_of_cash_flows() const
{
    return 1UL; // 1UL stands for 'one unsigned long'
}

MJArray PathDependentEuropean::possible_cash_flow_times() const
{
    MJArray tmp(1UL);
    tmp[0] = delivery_time;
    return tmp;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ! this method is (partially) responsible for determining payments of the option!!
// is uses the object of the PayOffCall subclass (thePayOff), which previously took
// as argument the final price of underlying. Since we are pricing arithmetic asian call,
// this time it takes mean spot price of underlying.
unsigned long PathDependentEuropean::cash_flows(const MJArray &spot_values,
                                             std::vector<CashFlow> &generated_flows) const
{
    //double last = spot_values[spot_values.size() - 1];
    double max = spot_values.max();
    generated_flows[0].time_index = 0UL;
    generated_flows[0].amount = the_payoff(max);

    return 1UL;
}

// standard method for cloning (making a copy of this object)
PathDependent *PathDependentEuropean::clone() const
{
    return new PathDependentEuropean(*this);
}
