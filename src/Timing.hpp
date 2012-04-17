#ifndef _INC_TIMING_hpp
#define _INC_TIMING_hpp

#include <map>
#include <string>
#include <cstdio>
#include <cmath>
#include <time.h>  // timespec
#include <sstream> // Defines also "timespec"
#include <cassert>
#include <stdint.h> // (u)int64_t

// log() is defined in StdCout.hpp
#include <StdCout.hpp>
#ifndef log
#define log printf
#endif // #ifndef log

// Forward declaration
class Timing;
// Timing global variable
extern std::map<std::string, Timing> Timings;

// **************************************************************
namespace timing
{
    const long TenToNine = 1000000000L; // 10^9

    const uint64_t years_to_days    = 365;
    const uint64_t days_to_hours    = 24;
    const uint64_t hours_to_min     = 60;
    const uint64_t min_to_sec       = 60;

    const uint64_t years_to_hours   = years_to_days * days_to_hours;
    const uint64_t years_to_min     = years_to_days * days_to_hours * hours_to_min;
    const uint64_t years_to_sec     = years_to_days * days_to_hours * hours_to_min * min_to_sec;

    const uint64_t days_to_min      = days_to_hours * hours_to_min;
    const uint64_t days_to_sec      = days_to_hours * hours_to_min * min_to_sec;

    const uint64_t hours_to_sec     = hours_to_min * min_to_sec;

    const double sec_to_nanosec     = 1.0e9;
    const double nanosec_to_sec     = 1.0 / sec_to_nanosec;
}

// **************************************************************
class Timer
{
private:
    timespec timer;

public:

    time_t Get_sec()  const;
    long   Get_nsec() const;

    Timer();
    void Clear();

    Timer operator+(const Timer &other);
    Timer operator-(const Timer &other);
    void Add_sec(time_t seconds);
    void Add_nsec(long nanseconds);
    void Get_Current_Time();
    void Print() const;
};

// **************************************************************
class Timing
{
    private:
        bool    is_initialized;
        Timer start;
        Timer end;
        Timer duration;

    public:
        Timing();
        void Reset_Duration();
        void Reset_Timer();
        void Update_Duration();
        void Add_Seconds(const double seconds);
        time_t Get_Duration_Seconds();
        long Get_Duration_NanoSeconds();
        double Get_Duration();
        double Calculate_Duration();
        uint64_t Duration_Years();
        uint64_t Duration_Days();
        uint64_t Duration_Hours();
        uint64_t Duration_Minutes();
        uint64_t Duration_Seconds();
        std::string Duration_Human_Readable();
        void Print() const;
};

namespace timing
{
    // **************************************************************
    void Wait(const double seconds);
    void Print_Timing_Info(const double nt);
    std::string Calculate_ETA(const double time, const double duration);

    // See Git_Info.cpp (generated dynamically from Git_Info.cpp_template & Makefile.rules)
    void Log_Git_Info(std::string basename = "");

    // **************************************************************
    template <class Integer>
    std::string IntToStr(const Integer integer, const int width = 0, const char fill = ' ')
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

#endif // #ifndef _INC_TIMING_hpp

// ********** End of file ***************************************
