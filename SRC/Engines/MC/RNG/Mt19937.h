#pragma once
#include "RngBase.h"
#include <random>

class Mt19937Rng : public RngBase
{
    private:
        std::mt19937_64 rng_;
        std::normal_distribution<double> norm_;

    public:
        Mt19937Rng(unsigned long seed);
        // oveeriding the implementation of base
        void get_gaussians(std::vector<double>& z) override;
};