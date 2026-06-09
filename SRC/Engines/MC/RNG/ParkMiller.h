#pragma once
#include "random2.h"

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

class RandomParkMiller : public RandomBase
{
    private:
        ParkMiller InnerGenerator;
        unsigned long InitialSeed;
        double Reciprocal;
    public:
        RandomParkMiller(unsigned long Dimensionality, unsigned long Seed = 1);

        virtual RandomBase *clone() const;
        virtual void get_uniforms(MJArray &variates);
        virtual void skip(unsigned long numberOfPaths);
        virtual void set_seed(unsigned long Seed);
        virtual void reset();
        virtual void reset_dimensionality(unsigned long NewDimensionality);
        virtual ~RandomParkMiller();
};