// RngBase is a base clase (abstract) which gives interface for the actual 
// impleementation of random generators. this encompasses the in house implementation
// of Park MIller as well as mt19937

//             RngBase                ← the interface / "the contract": get_gaussians()
//             ▲      ▲
//        IS-A │      │ IS-A          ← inheritance  (": public RngBase")
//             │      │
//      Mt19937Rng   ParkMillerRng    ← the two WRAPPERS (they speak RngBase)
//             │      │
//       HAS-A │      │ HAS-A          ← composition  (a member inside)
//             ▼      ▼
//    std::mt19937   ParkMiller       ← the raw number-cranks (know nothing about RngBase)

#pragma once
#include <vector>

class RngBase 
{
    public:
        virtual void get_gaussians(std::vector<double>& z) = 0;
        virtual ~RngBase() {}
};