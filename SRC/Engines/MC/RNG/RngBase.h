// RngBase is a base clase (abstract) which gives interface for the actual 
// impleementation of random generators. this encompasses the in house implementation
// of Park MIller as well as mt19937

#pragma once
#include <vector>

class RngBase 
{
    public:
        virtual void get_gaussians(std::vector<double>& z) = 0;
        virtual ~RngBase() {}
};