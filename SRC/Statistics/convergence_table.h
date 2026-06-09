#pragma once
#include "statistics_mc.h"
#include <memory>

class ConvergenceTable : public StatisticsMC
{
public:
    ConvergenceTable(const std::shared_ptr<StatisticsMC> &Inner_);

    virtual StatisticsMC *clone() const;
    virtual void dump_one_result(double result);
    virtual std::vector<MCResult> get_results_so_far() const;
    virtual void reset();

private:
    std::shared_ptr<StatisticsMC> Inner;
    std::vector<MCResult> results_so_far;
    unsigned long stopping_point;
    unsigned long paths_done;
};