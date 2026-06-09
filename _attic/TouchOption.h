// For later
// #pragma once
// #include "payoff.h"
// #include <memory>
// #include "instrument.h"

// enum class BarrierType { DownAndOut, UpAndOut, DownAndIn, UpandIn};

// class BarrierOption: public Instrument
// {
//     private:
//         double expiry_;
//         std::shared_ptr<Payoff> payoff_;
//         double barrier_;
//         BarrierType barrier_type_;
//     public:
//         BarrierOption(std::shared_ptr<Payoff> payoff, double expiry, double barrier, BarrierType barrier_type);
//         double expiry() const;
//         double barrier() const;
//         BarrierType barrier_type() const;
//         const Payoff& payoff() const;
// };