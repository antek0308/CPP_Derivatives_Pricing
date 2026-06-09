#include "statistics_mc.h"
using namespace std;

StatisticsMean::StatisticsMean() : running_sum(0.0), paths_done(0UL)
{
}

void StatisticsMean::dump_one_result(double result)
{
    paths_done++;
    running_sum += result;
}

std::vector<MCResult> StatisticsMean::get_results_so_far() const
{
    MCResult r;
    r.mean = running_sum / paths_done;
    r.paths = paths_done;
    r.stdError = 0;
    r.ciLow = 0;
    r.ciHigh = 0;
    return {r};
}

StatisticsMC *StatisticsMean::clone() const
{
    return new StatisticsMean(*this);
}

void StatisticsMean::reset()
{
    paths_done = 0UL;
    running_sum = 0.0;
}
