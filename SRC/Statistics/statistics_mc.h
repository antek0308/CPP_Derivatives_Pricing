#pragma once
#include <vector>

// ---------------------------------------------------------------------------
// MCResult is now the "universal currency" every gatherer returns.
// It used to be an anonymous std::vector<double> row (hence the ugly res[0][0]).
// We give the columns names + a path count, and we put it HERE because the
// base-class contract below returns it, so every gatherer must see one and the
// same definition (defining it twice = "redefinition" error).
// ---------------------------------------------------------------------------
struct MCResult
{
    double mean;
    double stdError;
    double ciLow;
    double ciHigh;
    unsigned long paths; // used by the convergence table
};

class StatisticsMC
{
    private:

    public:
        StatisticsMC() {};
        virtual void dump_one_result(double result) = 0;
        // OLD universal interface: an unnamed table of doubles -> forced ugly res[0][0]
        // virtual std::vector<std::vector<double>> get_results_so_far() const = 0;
        // NEW: same SHAPE (outer vector = rows/snapshots) but each row is a named
        // MCResult. A plain gatherer returns 1 row; the convergence table returns N.
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