#ifndef _INC_TIMING_hpp
#define _INC_TIMING_hpp

#include <map>
#include <string>

#include <cstdio>
#include <iostream>
#include <cmath>
#include <sstream>
#include <stdint.h> // (u)int64_t

#include <time.h>

#include <StdCout.hpp>

const long TenToNine = 1000000000L; // 10^9


namespace TimingNamespace
{
    template <class Double>
    void Wait(const Double duration_sec)
    {

class Timer
{
private:
    timespec timer;

public:

    time_t Get_sec()  const { return timer.tv_sec;  }
    long   Get_nsec() const { return timer.tv_nsec; }

    void Clear()
    {
        timer.tv_sec  = 0;
        timer.tv_nsec = 0;
    }

    Timer()
    {
        Clear();
    }

    Timer operator+(const Timer &other)
    /**
     * Add two timers
     */
    {
        timer.tv_sec  = timer.tv_sec  + other.timer.tv_sec;
        timer.tv_nsec = timer.tv_nsec + other.timer.tv_nsec;

        // tv_nsec stores the nansecond between tv_sec and the next second.
        // Increment the number of seconds if needed
        if (timer.tv_nsec >= TenToNine)
        {
            timer.tv_sec++;
            timer.tv_nsec = timer.tv_nsec - TenToNine;
        }

        return *this;
    }

    Timer operator-(const Timer &other)
    {
        // Prevent overflow in the case that tv_nsec > other.tv_nsec
        if (timer.tv_nsec - other.timer.tv_nsec < 0)
        {
            timer.tv_sec  = timer.tv_sec - other.timer.tv_sec - 1;
            timer.tv_nsec = TenToNine + timer.tv_nsec - other.timer.tv_nsec;
        }
        else
        {
            timer.tv_sec  = timer.tv_sec  - other.timer.tv_sec;
            timer.tv_nsec = timer.tv_nsec - other.timer.tv_nsec;
        }

        return *this;
    }

    void Get_Current_Time()
    {
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &(this->timer));
    }
};

template <class Double>
class Timing
{
    private:
        bool    is_initialized;
        Timer start;
        Timer end;
        Timer duration;

    public:
        // ******************************************************
        Timing()
        {
            is_initialized  = false;
        }

        // ******************************************************
        void Reset_Duration()
        {
            start.Clear();
            end.Clear();
            duration.Clear();
            Reset_Timer();
        }

        // ******************************************************
        void Reset_Timer()
        {
            is_initialized = true;
            start.Get_Current_Time();
        }

        // ******************************************************
        void Update_Duration()
        {
            assert(is_initialized);

            end.Get_Current_Time();
            duration += (end - start);
        }

        // ******************************************************
        time_t Get_Duration_Seconds()
        /**
         * Returns timer's elapsed duration in seconds (integer representation).
         */
        {
            return duration.Get_sec();
        }

        // ******************************************************
        long Get_Duration_NanoSeconds()
        /**
         * Returns timer's elapsed duration in nanoseconds (integer representation).
         */
        {
            return duration.Get_nsec();
        }

        // ******************************************************
        Double Get_Duration()
        /**
         * Returns timer's elapsed duration in seconds (float representation).
         */
        {
            return Double(Get_Duration_Seconds() + Get_Duration_NanoSeconds());
        }

};



#endif // #ifndef _INC_TIMING_hpp

// ********** End of file ***************************************
