#pragma once


// Data Collector for BS Process
class BlackScholesProcess
{
    private:
        double r_;
        double d_;
        double vol_;
        double spot_;
    public:
        BlackScholesProcess(double spot, double vol, double r, double d);
        double spot() const;
        double d() const;
        double vol() const;
        double r() const;
};