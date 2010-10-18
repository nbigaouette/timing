#ifndef _INC_TIMING_hpp
#define _INC_TIMING_hpp

#include <map>
#include <string>

#include <FloatType.hpp>

#include <sys/time.h>
typedef timeval Timeval;

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
        void    Update_Duration(fdouble elapsed_sec);
        fdouble  Get_Duration();
};

// Timing global variable
extern std::map<std::string, Timing> Timings;

std::string Get_Elapsed_Time_String(const fdouble seconds);

#endif // #ifndef _INC_TIMING_hpp

// ********** End of file ***************************************
