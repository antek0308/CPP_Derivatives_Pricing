//What does my engine want? → get_gaussians(vector&) (the RngBase contract).
// What raw source do I have? → a thing that emits raw numbers (ParkMiller integers; 
// or std::mt19937 bits).
// They don't match → write a wrapper: class XxxRng : public RngBase that holds the 
// raw source and, in get_gaussians, asks it for numbers and converts them to gaussians.
// Construct it from a seed (hide the raw source inside).

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