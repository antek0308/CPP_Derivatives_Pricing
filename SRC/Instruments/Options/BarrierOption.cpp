#include "BarrierOption.h"
#include <memory>
#include <limits> // std::numeric_limits<double>::infinity()

// Implementation of Barrier Option
// I decided to make eventually double barrier option a default case as it is neater and double barrier
// decay to single barrier with correct parmeters

BarrierOption::BarrierOption(std::shared_ptr<Payoff> payoff, double expiry, 
                             double lower_barrier, double upper_barrier, Knock barrier_type)
    : expiry_(expiry), payoff_(payoff), lower_barrier_(lower_barrier), upper_barrier_(upper_barrier),
    barrier_type_(barrier_type)
{
}

double BarrierOption::expiry() const { return expiry_; }
double BarrierOption::lower_barrier() const { return lower_barrier_; }
double BarrierOption::upper_barrier() const { return upper_barrier_; }
Knock BarrierOption::barrier_type() const { return barrier_type_; }
const Payoff& BarrierOption::payoff() const { return *payoff_; }

// As in BarrierOption.h:
// here are named makers to make pricing single barrier more convinient and have one class for single and
// double barriers. If it was not implemented one would need to rember to pass inf or -inf for upper or lower barrier
// to make pricing correct, which was not elegant
namespace { const double INF = std::numeric_limits<double>::infinity(); }

BarrierOption BarrierOption::downOut(std::shared_ptr<Payoff> payoff, double expiry, double barrier)
{   // dies if S falls through lower barrier; inf for upper one and analogically all the types
    return BarrierOption(payoff, expiry, barrier, INF, Knock::Out);
}
BarrierOption BarrierOption::upOut(std::shared_ptr<Payoff> payoff, double expiry, double barrier)
{
    return BarrierOption(payoff, expiry, -INF, barrier, Knock::Out);
}
BarrierOption BarrierOption::doubleOut(std::shared_ptr<Payoff> payoff, double expiry, double lower, double upper)
{
    return BarrierOption(payoff, expiry, lower, upper, Knock::Out);
}
BarrierOption BarrierOption::downIn(std::shared_ptr<Payoff> payoff, double expiry, double barrier)
{
    return BarrierOption(payoff, expiry, barrier, INF, Knock::In);
}
BarrierOption BarrierOption::upIn(std::shared_ptr<Payoff> payoff, double expiry, double barrier)
{
    return BarrierOption(payoff, expiry, -INF, barrier, Knock::In);
}
BarrierOption BarrierOption::doubleIn(std::shared_ptr<Payoff> payoff, double expiry, double lower, double upper)
{
    return BarrierOption(payoff, expiry, lower, upper, Knock::In);
}