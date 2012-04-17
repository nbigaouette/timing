/***************************************************************
 *
 *
 ***************************************************************/

#include <stdint.h> // uint64_t
#include <limits>  // http://www.cplusplus.com/reference/std/limits/numeric_limits/
#include <iostream>
#include <cstdlib>

#include "Timing.hpp"


// **************************************************************
int main(int argc, char *argv[])
{
    timing::Log_Git_Info();

    timespec temp;
    std::cout << "sizeof(end.tv_nsec) = " << sizeof(temp.tv_nsec) << "\n";
    std::cout << "sizeof(end.tv_sec) = " << sizeof(temp.tv_sec) << "\n";
    temp.tv_nsec = 0;
    temp.tv_sec = 0;
    temp.tv_nsec = temp.tv_nsec - 1;
    temp.tv_sec = temp.tv_sec - 1;
    std::cout << "end.tv_nsec = " << temp.tv_nsec << "\n";
    std::cout << "end.tv_sec = " << temp.tv_sec << "\n";
    std::cout << "std::numeric_limits<time_t>::max() =  " << std::numeric_limits<time_t>::max() << "\n";
    std::cout << "std::numeric_limits<long>::max()   =  " << std::numeric_limits<long>::max() << "\n";
    std::cout << "std::numeric_limits<time_t>::min() = " << std::numeric_limits<time_t>::min() << "\n";
    std::cout << "std::numeric_limits<long>::min()   = " << std::numeric_limits<long>::min() << "\n";

    const uint64_t N = 500000000;

    TIMER_START("Total", Timing_Total);

    TIMER_START("Test1", Timing_Test1);
    int itemp = 0;
    for (uint64_t i = 0 ; i < N ; i++)
        itemp += itemp;
    TIMER_STOP("Test1", Timing_Test1);

    TIMER_START("Wait", Timing_Wait);
    timing::Wait(1.0);
    TIMER_STOP("Wait", Timing_Wait);

    TIMER_STOP("Total", Timing_Total);

    const double duration = double(100*N);
    const double time = 0.0;
    timing::Eta ETA(time, duration);
    std::cout << "ETA: for 100xN = " << 100*N << " iterations: " << ETA.Get_ETA(0.1*duration) << "\n";

    timing::Print(double(N));

    return EXIT_SUCCESS;
}

