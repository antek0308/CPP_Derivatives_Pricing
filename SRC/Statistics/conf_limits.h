#ifndef CONF_LIMITS_H
#define CONF_LIMITS_H

#include "statistics_mc.h"
#include <vector>


class ConfidenceLimits : public StatisticsMC
{

public:
    // z_value is the normal quantile. 1.645 -> 90%, 1.96 -> 95%
    ConfidenceLimits(double z_value_ = 1.645);
    virtual void dump_one_result(double result);
    // virtual std::vector<std::vector<double>> get_results_so_far() const;
    virtual std::vector<MCResult> get_results_so_far() const; // 1-row table (mean/se/CI for all paths)
    virtual StatisticsMC *clone() const;
    virtual void reset();

private:
    std::vector<double> payoffs;
    double z_value;
};

#endif
