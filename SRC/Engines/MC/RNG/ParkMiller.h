// ParkMiller is the raw integer generator (linear congruential).
// ParkMillerRng wraps it and turns the integers into gaussians, so it fits the RngBase
// interface and can be used by the engine the same way as Mt19937Rng.

#pragma once
#include "RngBase.h"

class ParkMiller
{
    private:
        long seed;

    public:
        ParkMiller(long Seed = 1);

        long get_one_random_integer();
        void set_seed(long Seed);

        static unsigned long max();
        static unsigned long min();
};

// ParkMillerRng uses the interface of RngBase, but the user does not need to know
// the impleemntation of ParkMiller, so it is initilised with 
class ParkMillerRng : public RngBase
{
    private:
        ParkMiller core_;
    
    public:
        // Changed 
        ParkMillerRng(long seed);
        void get_gaussians(std::vector<double>& z) override;
};