#include "statistics_mc.h"
#include "conf_limits.h"
#include "helpers.h"
#include <cmath>

ConfidenceLimits::ConfidenceLimits(double z_value_)
    : payoffs(0), z_value(z_value_)
{
}

void ConfidenceLimits::dump_one_result(double result)
{
    payoffs.push_back(result);
}

std::vector<MCResult> ConfidenceLimits::get_results_so_far() const
{
    MCResult ResultsStruct;
    ResultsStruct.mean = get_vec_mean(payoffs);
    ResultsStruct.stdError = get_vec_stddev(payoffs) / pow(payoffs.size(), 0.5);
    ResultsStruct.ciLow = ResultsStruct.mean - z_value * ResultsStruct.stdError;
    ResultsStruct.ciHigh = ResultsStruct.mean + z_value * ResultsStruct.stdError;
    ResultsStruct.paths = payoffs.size();

    // ResultsStruct won't match the vector return type, must be in {}
    return { ResultsStruct };
}

StatisticsMC *ConfidenceLimits::clone() const
{
    return new ConfidenceLimits(*this);
}

void ConfidenceLimits::reset()
{
    payoffs.clear();
}
