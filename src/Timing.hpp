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
#include <fstream>

// Quote something, usefull to quote a macro's value
#ifndef _QUOTEME
#define _QUOTEME(x) #x
#endif // #ifndef _QUOTEME
#ifndef QUOTEME
#define QUOTEME(x) _QUOTEME(x)
#endif // #ifndef QUOTEME

// **************************************************************
// Define useful macros.
// The two macros TIMER_START() and TIMER_STOP() can be used to
// allow easy enabling/disabling of any timer measurement. Just
// define "DISABLE_TIMING" before including Timing.hpp (either
// with a #define or with -DDISABLE_TIMING CFLAG) and these macros
// won't do anything, as will ETA class and timing::Print().
#ifndef DISABLE_TIMING
    #define TIMER_START(name, Timer_name) \
        static timing::Timer &Timer_name = timing::New_Timer(name, QUOTEME(Timer_name)); \
        Timer_name.Start();
    #define TIMER_STOP(name, Timer_name) \
        Timer_name.Stop();
    #define TIMERS_ENABLE_OUTPUT(output_folder) \
        timing::Enable_Timers_Output(output_folder);
    #define TIMERS_SET_STEP(step) \
        timing::Set_Timers_Step(step);
#else // #ifndef DISABLE_TIMING
    #define TIMER_START(name, Timer_name)       {}
    #define TIMER_STOP(name, Timer_name)        {}
    #define TIMERS_ENABLE_OUTPUT(output_folder) {}
    #define TIMERS_SET_STEP(step)               {}
#endif // #ifndef DISABLE_TIMING

// **************************************************************
namespace timing
{
    // Forward declarations
    class Clock;
    class Timer;
    class Eta;

    // **********************************************************
    Timer & New_Timer(const std::string &full_name, const std::string &strict_name);
    void Wait(const double seconds);
    void Print_N_Times(const std::string x, const size_t N, const bool newline = true);
    void _Print(const uint64_t nt, const size_t terminal_width);
#ifndef DISABLE_TIMING
    inline void Print(const uint64_t nt, const size_t terminal_width = 1) { _Print(nt, terminal_width);   }
#else // #ifndef DISABLE_TIMING
    inline void Print(const uint64_t nt, const size_t terminal_width = 1) { /* Don't do anything */       }
#endif // #ifndef DISABLE_TIMING

    void Stop_All_Timers();
    void Enable_Timers_Output(const std::string &_output_folder);
    void Set_Timers_Step(const uint64_t _step);

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

            std::string Get_Time() const;
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
            bool is_started;
            uint64_t counter;
            Clock start;
            Clock end;
            Clock duration;
            Clock current_duration;

            // Used in output
            std::string   name;
            std::string   output_filename;
            std::ofstream output_file;
            bool          output_has_been_performed;

        public:
            Timer();
            Timer(const Timer &other);
            void Set_Name(const std::string &_full_name, const std::string &_strict_name);
            void Clear();
            void Start();
            void Stop();
            uint64_t Get_Counter() const;
            void Add_Seconds(const double seconds);
            time_t Get_Duration_Seconds() const;
            long Get_Duration_NanoSeconds() const;
            double Get_Duration() const;
            double Get_Current_Duration() const;
            void Update_Duration();
            uint64_t Duration_Years();
            uint64_t Duration_Days();
            uint64_t Duration_Hours();
            uint64_t Duration_Minutes();
            uint64_t Duration_Seconds();
            std::string Duration_Human_Readable();
            void Print() const;

            // Stop_All_Timers() needs to reset TimerTotal's duration
            friend void Stop_All_Timers();
            // New_Timer() acts as a constructor-like
            friend Timer & New_Timer(const std::string &_name);
    };

    // **********************************************************
    class Eta
    {
        private:
            double first_time;
            double duration;

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
