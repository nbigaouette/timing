/***************************************************************
 *
 *
 ***************************************************************/

#include <stdint.h> // uint64_t
#include <limits>  // http://www.cplusplus.com/reference/std/limits/numeric_limits/
#include <iostream>
#include <cstdlib>

#include "Timing.hpp"

std::map<std::string, Timing> Timings;

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
    timing::Calculate_ETA(0, double(10*N));

    Timings["Total"].Reset_Timer();

    Timings["Test1"].Reset_Timer();
    int itemp = 0;
    for (uint64_t i = 0 ; i < N ; i++)
        itemp += itemp;
    Timings["Test1"].Update_Duration();

    Timings["Wait"].Reset_Timer();
    timing::Wait(1.0);
    Timings["Wait"].Update_Duration();

    Timings["Total"].Update_Duration();

    // Bogus ETA
    std::cout << "ETA: for 100xN = " << 10*N << " iterations: " << timing::Calculate_ETA(double(N), double(10*N)) << "\n";

    timing::Print_Timing_Info(double(N));

    return EXIT_SUCCESS;
}

