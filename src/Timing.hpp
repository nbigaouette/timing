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
    }

    // **************************************************************
    template <class Integer>
    inline std::string IntToStr(const Integer integer, const int width = 0, const char fill = ' ')
    {
        std::ostringstream MyStream;
        if (width != 0)
        {
            //MyStream << std::setw(width);
            //MyStream << std::setfill(fill);
            MyStream.width(width);
            MyStream.fill(fill);
        }
        MyStream << integer << std::flush;
        return (MyStream.str());
    }
}

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
            duration = duration + (end - start);
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
            return Double(Get_Duration_Seconds()) + Double(Get_Duration_NanoSeconds()) / Double(TenToNine);
        }

        // ******************************************************
        Double Calculate_Duration()
        {
            assert(is_initialized);

            Timer now;
            now.Get_Current_Time();
            duration = now - start;
            return Get_Duration();
        }
};


// **************************************************************
inline std::string Format_Seconds_Human_Readable(const double s)
{
    double tmp = s;

    const unsigned int days = (unsigned int) std::floor(tmp / double(86400)); // 86400 seconds per day
    tmp = std::max(0.0, tmp - double(days*86400));
    const unsigned int hours = (unsigned int) std::floor(tmp / double(3600));  // 3600 seconds per hour
    tmp = std::max(0.0, tmp - double(hours*3600));
    const unsigned int minutes = (unsigned int) std::floor(tmp / double(60));    // 60 seconds per minute
    tmp = std::max(0.0, tmp - double(minutes*60));
    const unsigned int seconds = (unsigned int) tmp;

    std::string s_string;
    if (days != 0)
        s_string += TimingNamespace::IntToStr(days) + std::string("d");
    if (hours != 0 or days != 0)
        s_string += TimingNamespace::IntToStr(hours,2,'0') + std::string("h");
    if (minutes != 0 or hours != 0 or days != 0)
        s_string += TimingNamespace::IntToStr(minutes,2,'0') + std::string("m");
    s_string += TimingNamespace::IntToStr(seconds,2,'0') + std::string("s");

    return s_string;
}

// **************************************************************
template <class Double>
void Print_Timing_Info(const Double nt)
{
    const std::string s("                   ");
    bool total_found = false;
    std::string total_key;

}


#endif // #ifndef _INC_TIMING_hpp

// ********** End of file ***************************************
