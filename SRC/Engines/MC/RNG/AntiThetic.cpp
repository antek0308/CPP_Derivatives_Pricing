#pragma once
#include "AntiThetic.h"

AntiThetic::AntiThetic(std::shared_ptr<RngBase> inner)
    : inner_(inner), fresh_(true)
{
}

void AntiThetic::get_gaussians(std::vector<double>& z)
{
    if (fresh_)
    {
        inner_->get_gaussians(z); // calling the wrapper for new variable
        cached_ = z;// saving it
    }
    else
    {
        for (unsigned long i = 0; i < z.size(); i++)
            z[i] = -cached_[i]; // returning negative number
    }
    fresh_ = !fresh_; // next call
}