#ifndef _INC_TIMING_hpp
#define _INC_TIMING_hpp

#include <map>
#include <string>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <sstream>
#include <cassert>

#include <time.h>

// log() is defined in StdCout.hpp
#include <StdCout.hpp>
#ifndef log
#define log printf
#endif // #ifndef log

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

const double nanosec_to_sec   = TenToNine;
const double sec_to_nanosec   = 1.0 / nanosec_to_sec;

// Forward declaration
namespace TimingNamespace
{
    template <class Double>
    void Wait(const Double duration_sec);

    template <class Integer>
    std::string IntToStr(const Integer integer, const int width = 0, const char fill = ' ');
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
        /**
         * Get actual time and return number of seconds (float representation) since start of timer.
         */
        {
            assert(is_initialized);

            Timer now;
            now.Get_Current_Time();
            duration = now - start;
            return Get_Duration();
        }

        // **************************************************************
        uint64_t Duration_Years()
        /**
        * Return how many years.
        */
        {
            return uint64_t(std::floor(Get_Duration() / Double(years_to_sec)));
        }

        // **************************************************************
        uint64_t Duration_Days()
        /**
        * Return how many months (not including full years).
        */
        {
            const uint64_t years = Duration_Years();
            Double remaining_seconds = std::max(0.0, Get_Duration() - Double(years*years_to_sec));
            return uint64_t(std::floor(remaining_seconds / Double(days_to_sec)));
        }

        // **************************************************************
        uint64_t Duration_Hours()
        /**
        * Return how many hours (not including full years or days).
        */
        {
            const uint64_t years = Duration_Years();
            const uint64_t days  = Duration_Days();
            Double remaining_seconds = std::max(0.0, Get_Duration() - Double(years*years_to_sec) - Double(days*days_to_sec));
            return uint64_t(std::floor(remaining_seconds / Double(hours_to_sec)));
        }

        // **************************************************************
        uint64_t Duration_Minutes()
        /**
        * Return how many minutes (not including full years, days or hours).
        */
        {
            const uint64_t years = Duration_Years();
            const uint64_t days  = Duration_Days();
            const uint64_t hours = Duration_Hours();
            Double remaining_seconds = std::max(0.0, Get_Duration() - Double(years*years_to_sec) - Double(days*days_to_sec) - Double(hours*hours_to_sec));
            return uint64_t(std::floor(remaining_seconds / Double(min_to_sec)));
        }

        // **************************************************************
        uint64_t Duration_Seconds()
        /**
        * Return how many minutes (not including full years, days, hours or minutes).
        */
        {
            const uint64_t years    = Duration_Years();
            const uint64_t days     = Duration_Days();
            const uint64_t hours    = Duration_Hours();
            const uint64_t minutes  = Duration_Hours();
            Double remaining_seconds = std::max(0.0, Get_Duration() - Double(years*years_to_sec) - Double(days*days_to_sec) - Double(hours*hours_to_sec) - Double(minutes*min_to_sec));
            return uint64_t(std::floor(remaining_seconds));
        }

        std::string Duration_Human_Readable()
        /**
         * Return the duration in human readable format
         */
        {
            const uint64_t years    = Duration_Years();
            const uint64_t days     = Duration_Days();
            const uint64_t hours    = Duration_Hours();
            const uint64_t minutes  = Duration_Hours();
            const uint64_t seconds  = Duration_Seconds();

            std::string duration_string;
            if (days != 0)
                duration_string += TimingNamespace::IntToStr(days) + std::string("d");
            if (hours != 0 or days != 0)
                duration_string += TimingNamespace::IntToStr(hours,2,'0') + std::string("h");
            if (minutes != 0 or hours != 0 or days != 0)
                duration_string += TimingNamespace::IntToStr(minutes,2,'0') + std::string("m");
            duration_string += TimingNamespace::IntToStr(seconds,2,'0') + std::string("s");

            return duration_string;
        }
};

// Timing global variable
#ifdef FLOATTYPE_SINGLE
extern std::map<std::string, Timing<float> >  Timings;
#else  // #ifdef FLOATTYPE_SINGLE
extern std::map<std::string, Timing<double> > Timings;
#endif // #ifdef FLOATTYPE_SINGLE

namespace TimingNamespace
{
    // **************************************************************
    template <class Double>
    void Wait(const Double seconds)
    /**
     * Wait for a specific amount of time.
     * NOTE: The "Timing" class will NOT count this sleep time in its timer!
     */
    {
        timespec to_wait, remaining;
        to_wait.tv_sec = time_t(seconds);
        to_wait.tv_nsec = long((seconds - Double(to_wait.tv_sec)) * sec_to_nanosec);
        int return_value = nanosleep(&to_wait, &remaining);
        if (return_value != 0)
        {
            log("ERROR in Wait()\n");
            abort();
        }
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


// **************************************************************
template <class Double>
void Print_Timing_Info(const Double nt)
{
    const std::string s("                   ");
    bool total_found = false;
    std::string total_key;

    for (typename std::map<std::string, Timing<Double> >::iterator it = Timings.begin() ; it != Timings.end(); it++ )
    {
        if (it->first == "Total" || it->first == "total")
        {
            total_found = true;
            total_key = it->first;
        }
    }

    log("%s_______________________________________________________________________\n", s.c_str());
    log("%s|                  Timing of different code aspects                   |\n", s.c_str());
    log("%s|---------------------------------------------------------------------|\n", s.c_str());
    log("%s|       Code Aspect         |          Duration          | Percentage |\n", s.c_str());
    log("%s|                           |  seconds   | per time step | over total |\n", s.c_str());
    log("%s|---------------------------|------------|---------------|------------|\n", s.c_str());
    for (typename std::map<std::string, Timing<Double> >::iterator it = Timings.begin() ; it != Timings.end(); it++ )
    {
        if (it->first != total_key)
        {
            log("%s| %-25s | %10.5g | %13.6g | ", s.c_str(), it->first.c_str(), Timings[it->first].Get_Duration(), Timings[it->first].Get_Duration() / nt);
                if (total_found)
                {
                    log("%10.2f |\n", (Timings[it->first].Get_Duration() / Timings[total_key].Get_Duration())*100.0);
                }
                else
                    log("     -     |\n");
        }
    }
    if (total_found)
    {
        log("%s| %-25s | %10.5g | %13.6g | %10.2f |\n", s.c_str(), total_key.c_str(), Timings[total_key].Get_Duration(),
                                                        Timings[total_key].Get_Duration() / nt,
                                                        (Timings[total_key].Get_Duration() / Timings[total_key].Get_Duration())*100.0);
    }
    log("%s|---------------------------|-----------------------------------------|\n", s.c_str());
    log("%s| Total (human readable):   | %39s |\n", s.c_str(), Timings[total_key].Duration_Human_Readable().c_str());
    log("%s|---------------------------------------------------------------------|\n\n", s.c_str());


    time_t rawtime;
    time(&rawtime);
    const int timing_max_string_size = 1000;
    char date_out[timing_max_string_size];      // Output string
    struct tm *date_format; // Saves in Date format
    date_format = localtime(&rawtime);
    strftime(date_out, timing_max_string_size, "%A, %B %dth %Y, %Hh%M:%S (%Y%m%d%H%M%S)", date_format);
    log("\nEnding time and date:\n    %s\n", date_out);
}


#endif // #ifndef _INC_TIMING_hpp

// ********** End of file ***************************************
