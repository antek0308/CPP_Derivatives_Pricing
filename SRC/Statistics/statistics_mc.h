#pragma once
#include <vector>

struct MCResult
{
    double mean;
    double stdError;
    double ciLow;
    double ciHigh;
    unsigned long paths;
};

class StatisticsMC
{
    private:

    public:
        StatisticsMC() {};
        virtual void dump_one_result(double result) = 0;
        // OLD universal interface: an unnamed tale of doubles -> forced ugly res[0][0]
        // virtual std::vector<std::vector<double>> get_results_so_far() const = 0;
        virtual std::vector<MCResult> get_results_so_far() const = 0;
        virtual StatisticsMC *clone() const = 0;
        virtual ~StatisticsMC() {};
        virtual void reset() = 0;
};

class StatisticsMean : public StatisticsMC
{
    private:
        double running_sum;
        unsigned long paths_done;
        
    public:
        StatisticsMean();
        virtual void dump_one_result(double result);
        // virtual std::vector<std::vector<double>> get_results_so_far() const;
        virtual std::vector<MCResult> get_results_so_far() const; // must match the base signature
        virtual StatisticsMC *clone() const;
        virtual void reset();
};