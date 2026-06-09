#pragma once
#include "RngBase.h"
#include <memory>
#include <vector>
// Decorator, which can wrap ParkMiller or Mt19937

// The biggest caveat here is that using anthitetiec variates trick, the confidence interval
// is no longer completly valid. The confidence limit is somehow the lower limit, but addding
// 2x times variables does not decrease the standard error as it would have done in independant 
// case, i.e. without anthitetic

class AntiThetic : public RngBase
{
    private:
        std::shared_ptr<RngBase> inner_; // has-a RngBase (ParkMiller or Mt19937)
        std::vector<double> cached_;
        bool fresh_;
    public:
        AntiThetic(std::shared_ptr<RngBase> inner);
        void get_gaussians(std::vector<double>& z) override;
};