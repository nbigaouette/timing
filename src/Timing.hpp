#ifndef _INC_TIMING_hpp
#define _INC_TIMING_hpp

#include <map>
#include <string>

#include <FloatType.hpp>

#ifdef CALCULATE_TIMING
#include <sys/time.h>
typedef timeval Timeval;
#else  // #ifdef CALCULATE_TIMING
typedef fdouble  Timeval;
#endif // #ifdef CALCULATE_TIMING

class Timing
{
    private:
        Timeval time_info;  // Time information
        fdouble  duration;   // Duration in seconds

    public:
                Timing();
        void    Reset_Duration();
        void    Reset_Timer();
        fdouble  Get_Elapsed_Time();
        void    Update_Duration();
        fdouble  Get_Duration();
};

// Timing global variable
extern std::map<std::string, Timing> Timings;

std::string Get_Elapsed_Time_String(const fdouble seconds);

#endif // #ifndef _INC_TIMING_hpp

// ********** End of file ***************************************
