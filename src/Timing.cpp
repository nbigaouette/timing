
#include "Timing.hpp"

#include <cstdlib>

using namespace timing;

// **************************************************************
time_t Timer::Get_sec() const
{
    return timer.tv_sec;
}

// **************************************************************
long Timer::Get_nsec() const
{
    return timer.tv_nsec;
}

// **************************************************************
void Timer::Clear()
{
    timer.tv_sec  = 0;
    timer.tv_nsec = 0;
}

// **************************************************************
Timer::Timer()
{
    Clear();
}

// **************************************************************
Timer Timer::operator+(const Timer &other)
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

// **************************************************************
Timer Timer::operator-(const Timer &other)
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

// **************************************************************
void Timer::Add_sec(time_t seconds)
{
    timer.tv_sec += seconds;
}

// **************************************************************
void Timer::Add_nsec(long nanoseconds)
{
    timer.tv_nsec += nanoseconds;
}

// **************************************************************
void Timer::Get_Current_Time()
{
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &(this->timer));
}



// **************************************************************
Timing::Timing()
{
    is_initialized  = false;
}

// **************************************************************
void Timing::Reset_Duration()
{
    start.Clear();
    end.Clear();
    duration.Clear();
    Reset_Timer();
}

// **************************************************************
void Timing::Reset_Timer()
{
    is_initialized = true;
    start.Get_Current_Time();
}

// **************************************************************
void Timing::Update_Duration()
{
    assert(is_initialized);

    end.Get_Current_Time();
    duration = duration + (end - start);
}

// **************************************************************
time_t Timing::Get_Duration_Seconds()
/**
 * Returns timer's elapsed duration in seconds (integer representation).
 */
{
    return duration.Get_sec();
}

// **************************************************************
long Timing::Get_Duration_NanoSeconds()
/**
 * Returns timer's elapsed duration in nanoseconds (integer representation).
 */
{
    return duration.Get_nsec();
}

// **************************************************************
double Timing::Get_Duration()
/**
 * Returns timer's elapsed duration in seconds (float representation).
 */
{
    return double(Get_Duration_Seconds()) + double(Get_Duration_NanoSeconds()) / double(TenToNine);
}

// **************************************************************
double Timing::Calculate_Duration()
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
uint64_t Timing::Duration_Years()
/**
 * Return how many years.
 */
{
    return uint64_t(std::floor(Get_Duration() / double(years_to_sec)));
}

// **************************************************************
uint64_t Timing::Duration_Days()
/**
 * Return how many months (not including full years).
 */
{
    const uint64_t years = Duration_Years();
    double remaining_seconds = std::max(0.0, Get_Duration() - double(years*years_to_sec));
    return uint64_t(std::floor(remaining_seconds / double(days_to_sec)));
}

// **************************************************************
uint64_t Timing::Duration_Hours()
/**
 * Return how many hours (not including full years or days).
 */
{
    const uint64_t years = Duration_Years();
    const uint64_t days  = Duration_Days();
    double remaining_seconds = std::max(0.0, Get_Duration() - double(years*years_to_sec) - double(days*days_to_sec));
    return uint64_t(std::floor(remaining_seconds / double(hours_to_sec)));
}

// **************************************************************
uint64_t Timing::Duration_Minutes()
/**
 * Return how many minutes (not including full years, days or hours).
 */
{
    const uint64_t years = Duration_Years();
    const uint64_t days  = Duration_Days();
    const uint64_t hours = Duration_Hours();
    double remaining_seconds = std::max(0.0, Get_Duration() - double(years*years_to_sec) - double(days*days_to_sec) - double(hours*hours_to_sec));
    return uint64_t(std::floor(remaining_seconds / double(min_to_sec)));
}

// **************************************************************
uint64_t Timing::Duration_Seconds()
/**
 * Return how many minutes (not including full years, days, hours or minutes).
 */
{
    const uint64_t years    = Duration_Years();
    const uint64_t days     = Duration_Days();
    const uint64_t hours    = Duration_Hours();
    const uint64_t minutes  = Duration_Hours();
    double remaining_seconds = std::max(0.0, Get_Duration() - double(years*years_to_sec) - double(days*days_to_sec) - double(hours*hours_to_sec) - double(minutes*min_to_sec));
    return uint64_t(std::floor(remaining_seconds));
}

// **************************************************************
std::string Timing::Duration_Human_Readable()
/**
 * Return the duration in human readable format
 */
{
    //const uint64_t years    = Duration_Years();
    const uint64_t days     = Duration_Days();
    const uint64_t hours    = Duration_Hours();
    const uint64_t minutes  = Duration_Hours();
    const uint64_t seconds  = Duration_Seconds();

    std::string duration_string;
    if (days != 0)
        duration_string += IntToStr(days) + std::string("d");
    if (hours != 0 or days != 0)
        duration_string += IntToStr(hours,2,'0') + std::string("h");
    if (minutes != 0 or hours != 0 or days != 0)
        duration_string += IntToStr(minutes,2,'0') + std::string("m");
    duration_string += IntToStr(seconds,2,'0') + std::string("s");

    return duration_string;
}

// **************************************************************
namespace timing
{
    // **********************************************************
    void Wait(const double seconds)
    /**
     * Wait for a specific amount of time.
     * NOTE: The "Timing" class will NOT count this sleep time in its timer!
     */
    {
        timespec to_wait, remaining;
        to_wait.tv_sec = time_t(seconds);
        to_wait.tv_nsec = long((seconds - double(to_wait.tv_sec)) * sec_to_nanosec);
        int return_value = nanosleep(&to_wait, &remaining);
        if (return_value != 0)
        {
            log("ERROR in Wait()\n");
            abort();
        }
    }

    // **********************************************************
    void Print_Timing_Info(const double nt)
    {
        const std::string s("                   ");
        bool total_found = false;
        std::string total_key;

        for (typename std::map<std::string, Timing>::iterator it = Timings.begin() ; it != Timings.end(); it++ )
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
        for (typename std::map<std::string, Timing>::iterator it = Timings.begin() ; it != Timings.end(); it++ )
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
}

// ********** End of file ***************************************
