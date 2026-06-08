#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include "park_miller.h"
#include "statistics_mc.h"
#include "conf_limits.h"
#include "path_dependent_barrier_asian.h"
#include "exotic_bs_engine.h"
#include "payoff4.h"

using namespace std;

int main()
{
    double expiry;
    double strike;
    double spot;
    double vol;
    double r;
    double d;
    double b_lower;
    double b_upper;
    unsigned long int number_of_paths;
    unsigned long int number_of_dates;
    unsigned long int number_of_reps; // how many times we redo the pricing fro part b)

    short unsigned decision = 2;
    while (decision > 1)
    {
        cout << "Do you want to take default parameters (0) or provide your own (1)?\n";
        cin >> decision;
    }

    if (decision == 0)
    {
        expiry = 0.5;
        strike = 100;
        spot = 95;
        vol = 0.2;
        r = 0.06;
        d = 0.0;
        b_lower = 90;
        b_upper = 125;
        number_of_paths = 100000;
        number_of_dates = 252 * expiry;
        number_of_reps = 500;
    }
    else
    {
        cout << "\nEnter expiry\n";
        cin >> expiry;

        cout << "\nEnter strike\n";
        cin >> strike;

        cout << "\nEnter spot\n";
        cin >> spot;

        cout << "\nEnter volatility\n";
        cin >> vol;

        cout << "\nEnter risk-free rate\n";
        cin >> r;

        cout << "\nEnter dividend rate\n";
        cin >> d;

        cout << "\nEnter lower barrier\n";
        cin >> b_lower;

        cout << "\nEnter upper barrier\n";
        cin >> b_upper;

        cout << "\nEnter number of intervals\n";
        cin >> number_of_dates;

        cout << "\nEnter number of paths\n";
        cin >> number_of_paths;

        cout << "\nEnter number of repetitions (part B)\n";
        cin >> number_of_reps;
    }

    // straddle payoff
    PayOffStraddle the_payoff(strike);

    MJArray times(number_of_dates);
    for (unsigned long i = 0; i < number_of_dates; i++)
    {
        times[i] = (i + 1.0) * expiry / number_of_dates;
    }

    ParametersConstant vol_param(vol);
    ParametersConstant r_param(r);
    ParametersConstant d_param(d);

    // the product: double barrier knock-out arithmetic asian straddle
    PathDependentBarrierAsian the_option(times, expiry, b_lower, b_upper, the_payoff);

    // a)
    // one pricing run + the 90% confidence limits.
    // plain park-miller (NO antithetic) so the payoffs are independent and the
    // standard error stddev/sqrt(n) is actually valid.
    RandomParkMiller generator(number_of_dates, 1);
    ExoticBSEngine the_engine(the_option, r_param, d_param, vol_param, generator, spot);

    ConfidenceLimits gatherer(1.645); // 1.645 = 90%
    the_engine.do_simulation(gatherer, number_of_paths);

    vector<vector<double>> res = gatherer.get_results_so_far();
    double price = res[0][0];
    double std_err = res[1][0];
    double lower = res[2][0];
    double upper = res[2][1];

    cout << fixed << setprecision(5);
    cout << "\nDouble barrier KO asian straddle\n";
    cout << "price: " << price << "\n";
    cout << "standard error: " << std_err << "\n";
    cout << "90% lower limit: " << lower << "\n";
    cout << "90% upper limit: " << upper << "\n";

    // b) I actually wasnt sure if I was suposed to show the payoffs of the one pricing or
    // redo the pricing few times. I decided to redo the pricing x times and show these prices as
    // histogram. I do it without anthitetic trick, because varaince reduction introduces dependance
    // and CLT depends on independance 
    RandomParkMiller generatorB(number_of_dates, 1);
    ExoticBSEngine engineB(the_option, r_param, d_param, vol_param, generatorB, spot);

    vector<double> estimates(number_of_reps);
    StatisticsMean mean_gatherer;

    for (unsigned long k = 0; k < number_of_reps; k++)
    {
        mean_gatherer.reset();
        engineB.do_simulation(mean_gatherer, number_of_paths);
        estimates[k] = mean_gatherer.get_results_so_far()[0][0];
    }

    // raw estimates to csv, then i do the histogram in excel
    ofstream f1("price_estimates.csv");
    f1 << "estimate\n";
    for (unsigned long k = 0; k < number_of_reps; k++)
        f1 << estimates[k] << "\n";
    f1.close();

    double mn = estimates[0];
    double mx = estimates[0];
    for (unsigned long k = 0; k < number_of_reps; k++)
    {
        if (estimates[k] < mn) mn = estimates[k];
        if (estimates[k] > mx) mx = estimates[k];
    }

    cout << "\nPART B\n";
    cout << "min estimate: " << mn << " , max estimate: " << mx << "\n";
    cout << "Prodcued: price_estimates.csv\n";

    return 0;
}
