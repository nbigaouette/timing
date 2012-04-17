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
// Not necessary though, it behaves the same as printf.
#include <StdCout.hpp>
#ifndef log
#define log printf
#endif // #ifndef log

// **************************************************************
// Define useful macros.
// The two macros TIMER_START() and TIMER_STOP() can be used to
// allow easy enabling/disabling of any timer measurement. Just
// define "DISABLE_TIMING" before including Timing.hpp (either
// with a #define or with -DDISABLE_TIMING CFLAG) and these macros
// won't do anything, as will ETA class and timing::Print().
#ifndef DISABLE_TIMING
    #define TIMER_START(name, Timer_name) \
        static timing::Timer &Timer_name = timing::New_Timer(name); \
        Timer_name.Reset();
    #define TIMER_STOP(name, Timer_name) \
        Timer_name.Update();
#else // #ifndef DISABLE_TIMING
    #define TIMER_START(name, Timer_name) {}
    #define TIMER_STOP(name, Timer_name)  {}
#endif // #ifndef DISABLE_TIMING

// **************************************************************
namespace timing
{

    // **********************************************************
    // See Git_Info.cpp (generated dynamically from Git_Info.cpp_template & Makefile.rules)
    void Log_Git_Info(std::string basename = "");

    // **********************************************************
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

    // **********************************************************
    class Clock
    {
        private:
            timespec timer;

        public:

            time_t Get_sec()  const;
            long   Get_nsec() const;

            Clock();
            void Clear();

            Clock operator+(const Clock &other);
            Clock operator-(const Clock &other);
            void Add_sec(time_t seconds);
            void Add_nsec(long nanseconds);
            void Get_Current_Time();
            void Print() const;
    };

    // **********************************************************
    class Timer
    {
        private:
            bool    is_initialized;
            Clock start;
            Clock end;
            Clock duration;

        public:
            Timer();
            void Reset_Duration();
            void Reset();
            void Update();
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

    // **********************************************************
    Timer & New_Timer(const std::string &name);
    void Wait(const double seconds);
    void _Print(const double nt);
#ifndef DISABLE_TIMING
    inline void Print(const double nt)  { _Print(nt);               }
#else // #ifndef DISABLE_TIMING
    inline void Print(const double nt)  { /* Don't do anything */   }
#endif // #ifndef DISABLE_TIMING

    // **********************************************************
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

    // **********************************************************
    class Eta
    {
        private:
            double first_time;
            double duration;
            Timer *Timer_Total_Ptr;

            std::string _Get_ETA(const double time) const;

        public:
            void Init(const double _first_time, const double _duration);
#ifndef DISABLE_TIMING
            Eta(const double _first_time, const double _duration)   { Init(_first_time, _duration); }
            std::string Get_ETA(const double time) const            { return _Get_ETA(time);        }
#else // #ifndef DISABLE_TIMING
            Eta(const double _first_time, const double _duration)   { /* Don't init anything */     }
            std::string Get_ETA(const double time) const            { return std::string("-");      }
#endif // #ifndef DISABLE_TIMING
    };
} // namespace timing

#endif // #ifndef _INC_TIMING_hpp

// ********** End of file ***************************************
