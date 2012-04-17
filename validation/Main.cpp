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

    static Timing &Timing_Total = timing::New_Timer("Total");
    Timing_Total.Reset_Timer();

    static Timing &Timing_Test1 = timing::New_Timer("Test1");
    Timing_Test1.Reset_Timer();
    int itemp = 0;
    for (uint64_t i = 0 ; i < N ; i++)
        itemp += itemp;
    Timing_Test1.Update_Duration();

    static Timing &Timing_Wait = timing::New_Timer("Wait");
    Timing_Wait.Reset_Timer();
    timing::Wait(1.0);
    Timing_Wait.Update_Duration();

    Timing_Total.Update_Duration();

    const double duration = double(100*N);
    const double time = 0.0;
    Eta ETA = Eta(time, duration);
    std::cout << "ETA: for 100xN = " << 100*N << " iterations: " << ETA.Get_ETA(0.1*duration) << "\n";
    std::cout << "&Timing_Total = " << &Timing_Total << "\n";

    timing::Print_Timing_Info(double(N));

    return EXIT_SUCCESS;
}

