/***************************************************************
 *
 *
 ***************************************************************/

#include <cstdlib>
#include <iostream>
#include <vector>
#include <stdint.h> // uint64_t
#include <limits>  // http://www.cplusplus.com/reference/std/limits/numeric_limits/

#include "src/Timing.hpp"

// #include "Version.hpp"
// #include "Git_Diff.hpp"

std::map<std::string, Timing<double> > Timings;

// **************************************************************
int main(int argc, char *argv[])
{
    std::cout << "Hello world!" << std::endl;
    std::cout << "sizeof(int) = " << sizeof(int) << std::endl;
    std::cout << "sizeof(long int) = " << sizeof(long int) << std::endl;
    std::cout << "sizeof(long long int) = " << sizeof(long long int) << std::endl;
    std::cout << "sizeof(unsigned int) = " << sizeof(unsigned int) << std::endl;
    std::cout << "sizeof(unsigned long int) = " << sizeof(unsigned long int) << std::endl;
    std::cout << "sizeof(unsigned long long int) = " << sizeof(unsigned long long int) << std::endl;
    std::cout << "sizeof(size_t) = " << sizeof(size_t) << std::endl;
    std::cout << "sizeof(uint64_t) = " << sizeof(uint64_t) << std::endl;
    std::cout << "sizeof(uint32_t) = " << sizeof(uint32_t) << std::endl;
    std::cout << "sizeof(int64_t) = " << sizeof(int64_t) << std::endl;
    std::cout << "sizeof(int32_t) = " << sizeof(int32_t) << std::endl;
    std::cout << "sizeof(std::vector<uint64_t>::size_type) = " << sizeof(std::vector<uint64_t>::size_type) << std::endl;
    std::cout << "sizeof(std::vector<double>::size_type) = " << sizeof(std::vector<double>::size_type) << std::endl;
    std::cout << "sizeof(std::vector<float>::size_type) = " << sizeof(std::vector<float>::size_type) << std::endl;

    const uint64_t max = std::numeric_limits<uint64_t>::max();
    const uint64_t maxp1 = max + uint64_t(1);
    const uint64_t zero = 0;
    const uint64_t zerom1 = zero - uint64_t(1);
    std::cout << "uint64_t max   = " << max << std::endl;
    std::cout << "uint64_t max+1 = " << maxp1 << std::endl;
    std::cout << "uint64_t 0     = " << zero << std::endl;
    std::cout << "uint64_t 0-1   = " << zerom1 << std::endl;

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

    Timings["Total"].Reset_Timer();

    Timings["Test1"].Reset_Timer();
    int itemp = 0;
    const int N = 242000000;
    for (int i = 0; i< N; i++)
        itemp += itemp;
    Timings["Test1"].Update_Duration();

    Timings["Total"].Update_Duration();

    Print_Timing_Info(double(N));

    return EXIT_SUCCESS;
}

