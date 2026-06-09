#include "path_dependent_barrier_asian.h"

PathDependentBarrierAsian::PathDependentBarrierAsian(const MJArray &look_at_times_,
                                                     double delivery_time_,
                                                     double b_lower_,
                                                     double b_upper_,
                                                     const PayOffBridge &the_payoff_)
    : PathDependent(look_at_times_),
      delivery_time(delivery_time_),
      b_lower(b_lower_),
      b_upper(b_upper_),
      the_payoff(the_payoff_),
      number_of_times(look_at_times_.size())
{
}

unsigned long PathDependentBarrierAsian::max_number_of_cash_flows() const
{
    return 1UL;
}

MJArray PathDependentBarrierAsian::possible_cash_flow_times() const
{
    MJArray tmp(1UL);
    tmp[0] = delivery_time;
    return tmp;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// same idea as the plain asian but first we check the barriers.
// we only look at the spot on the monitoring dates (discrete monitoring).
// if min of the path < lower barrier OR max of the path > upper barrier => knocked out => 0
unsigned long PathDependentBarrierAsian::cash_flows(const MJArray &spot_values,
                                                    std::vector<CashFlow> &generated_flows) const
{
    generated_flows[0].time_index = 0UL;

    if (spot_values.min() < b_lower || spot_values.max() > b_upper)
    {
        generated_flows[0].amount = 0.0;
        return 1UL;
    }

    // arithmetic asian payoff
    double sum = spot_values.sum();
    double mean = sum / number_of_times;

    generated_flows[0].amount = the_payoff(mean);

    return 1UL;
}

PathDependent *PathDependentBarrierAsian::clone() const
{
    return new PathDependentBarrierAsian(*this);
}
