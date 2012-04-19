/***************************************************************
 *
 *
 ***************************************************************/

#include <stdint.h> // uint64_t
#include <limits>  // http://www.cplusplus.com/reference/std/limits/numeric_limits/
#include <iostream>
#include <cstdlib>

// #define DISABLE_TIMING
#include "Timing.hpp"


// **************************************************************
int main(int argc, char *argv[])
{
    // Show some nice git informations about the working directory.
    timing::Log_Git_Info();

    // Enable saving the timing information in "output" folder.
    TIMERS_ENABLE_OUTPUT("output");

    // Do heavy calculation here
    const int max_t = 10000000;

    // Starting time is 0.0, end time is 3*max_t (two rounds of cosines and one of sines)
    static timing::Eta ETA = timing::Eta(0.0, double(3*max_t));

    // This will create a new timing object called "cosine".
    TIMER_START("cosine", Timing_Cosine);
    for (int t = 0 ; t < max_t ; t++)
    {
        TIMERS_SET_STEP(t);

        const double tmp = std::cos(t);

        // Print only 10 ETA, else we are flooded...
        if (t % (max_t/10) == 0)
            std::cout << "ETA: " << ETA.Get_ETA(double(t)) << "\n";
    }
    TIMER_STOP("cosine", Timing_Cosine);

    // This will create a new timing object called "sine".
    TIMER_START("sine", Timing_Sine);
    for (int t = 0 ; t < max_t ; t++)
    {
        TIMERS_SET_STEP(t);

        const double tmp = std::sin(t);

        // Print only 10 ETA, else we are flooded...
        if (t % (max_t/10) == 0)
            std::cout << "ETA: " << ETA.Get_ETA(double(max_t + t)) << "\n";
    }
    TIMER_STOP("sine", Timing_Sine);

    // Do another round of cosines. Note that the second
    // argument must be different.
    TIMER_START("cosine", Timing_Cosine2);
    for (int t = 0 ; t < max_t ; t++)
    {
        TIMERS_SET_STEP(t);

        const double tmp = std::cos(t);

        // Print only 10 ETA, else we are flooded...
        if (t % (max_t/10) == 0)
            std::cout << "ETA: " << ETA.Get_ETA(double(max_t + max_t + t)) << "\n";
    }
    TIMER_STOP("cosine", Timing_Cosine2);

    timing::Print(max_t);

    return EXIT_SUCCESS;
}

