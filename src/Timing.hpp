#ifndef _INC_TIMING_hpp
#define _INC_TIMING_hpp

#include <map>
#include <string>

#include <cstdio>
#include <iostream>
#include <cmath>
#include <sstream>
#include <stdint.h> // (u)int64_t


#include <StdCout.hpp>


namespace TimingNamespace
{
    template <class Double>
    void Wait(const Double duration_sec)
    {
        {
        }
    }

template <class Double>
class Timing
{
    private:
        bool    is_initialized;

    public:
        // ******************************************************
        Timing()
        {
        }

        // ******************************************************
        void Reset_Duration()
        {
            Reset_Timer();
        }

        // ******************************************************
        void Reset_Timer()
        {
            is_initialized = true;
        }

        // ******************************************************
        {
            assert(is_initialized);

        }

        // ******************************************************
        {
        }

        // ******************************************************
        {
        }

        // ******************************************************
        Double Get_Duration()
        {
        }

};



#endif // #ifndef _INC_TIMING_hpp

// ********** End of file ***************************************
