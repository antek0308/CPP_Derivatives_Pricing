// I cliped the ParkMIller so it is adpatable to my interface with Mt19937
// without the extesnive stuff peviously implemented.

#include "ParkMiller.h"
#include "normals.h"

const long a = 16807;
const long m = 2147483647;
const long q = 127773;
const long r = 2836;

ParkMiller::ParkMiller(long seed_) : seed(seed_)
{
    if (seed == 0)
        seed = 1;
}

ParkMillerRng::ParkMillerRng(long seed) : core_(seed) {}

void ParkMiller::set_seed(long seed_)
{
    seed = seed_;
    if (seed == 0)
        seed = 1;
}

unsigned long ParkMiller::max()
{
    return m - 1;
}

unsigned long ParkMiller::min()
{
    return 1;
}

long ParkMiller::get_one_random_integer()
{
    long k;
    k = seed / q;
    seed = a * (seed - k * q) - r * k;
    if (seed < 0)
        seed += m;
    return seed;
}


void ParkMillerRng::get_gaussians(std::vector<double>& z)
{
    for (double& zi : z)
    {
        long n = core_.get_one_random_integer();      // random integer in [1, m-1]
        double u = n / (1.0 + ParkMiller::max());      // scale to a uniform in (0,1)
        zi = InverseCumulativeNormal(u);               // turn the uniform into a N(0,1)
    }
}