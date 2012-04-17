
#include "Timing.hpp"

#include <cstdlib>

namespace timing
{
    std::map<std::string, Timer> TimersMap;

    // **********************************************************
    Timer & New_Timer(const std::string &name)
    {
        return TimersMap[name];
    }

    // **********************************************************
    time_t Clock::Get_sec() const
    {
        return timer.tv_sec;
    }

    // **********************************************************
    long Clock::Get_nsec() const
    {
        return timer.tv_nsec;
    }

    // **********************************************************
    void Clock::Clear()
    {
        timer.tv_sec  = 0;
        timer.tv_nsec = 0;
    }

    // **********************************************************
    Clock::Clock()
    {
        Clear();
    }

    // **********************************************************
    Clock Clock::operator+(const Clock &other)
    /**
    * Add two Clocks
    */
    {
        timer.tv_sec  = timer.tv_sec  + other.timer.tv_sec;
        timer.tv_nsec = timer.tv_nsec + other.timer.tv_nsec;

        // tv_nsec stores the nansecond between tv_sec and the next second.
        // Increment the number of seconds if needed
        if (timer.tv_nsec >= timing::TenToNine)
        {
            timer.tv_sec++;
            timer.tv_nsec = timer.tv_nsec - timing::TenToNine;
        }

        return *this;
    }

    // **********************************************************
    Clock Clock::operator-(const Clock &other)
    {
        // Prevent overflow in the case that tv_nsec > other.tv_nsec
        if (timer.tv_nsec - other.timer.tv_nsec < 0)
        {
            timer.tv_sec  = timer.tv_sec - other.timer.tv_sec - 1;
            timer.tv_nsec = timing::TenToNine + timer.tv_nsec - other.timer.tv_nsec;
        }
        else
        {
            timer.tv_sec  = timer.tv_sec  - other.timer.tv_sec;
            timer.tv_nsec = timer.tv_nsec - other.timer.tv_nsec;
        }

        return *this;
    }

    // **********************************************************
    void Clock::Add_sec(time_t seconds)
    {
        timer.tv_sec += seconds;
    }

    // **********************************************************
    void Clock::Add_nsec(long nanoseconds)
    {
        timer.tv_nsec += nanoseconds;
    }

    // **********************************************************
    void Clock::Get_Current_Time()
    {
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &(this->timer));
    }

    // **********************************************************
    void Clock::Print() const
    {
        log("  sec:  %ld\n", timer.tv_sec);
        log("  nsec: %ld\n", timer.tv_nsec);
    }

    // **********************************************************
    Timer::Timer()
    {
        is_initialized  = false;
    }

    // **********************************************************
    void Timer::Reset_Duration()
    {
        start.Clear();
        end.Clear();
        duration.Clear();
        Reset();
    }

    // **********************************************************
    void Timer::Reset()
    {
        is_initialized = true;
        start.Get_Current_Time();
    }

    // **********************************************************
    void Timer::Update()
    {
        assert(is_initialized);

        end.Get_Current_Time();
        duration = duration + (end - start);
    }

    // **********************************************************
    void Timer::Add_Seconds(const double seconds)
    /**
    * Add a number of seconds to the current Clock.
    */
    {
        time_t nb_seconds = time_t(std::floor(seconds));
        long nanoseconds = long((seconds - double(nb_seconds)) * timing::sec_to_nanosec);
        end.Add_sec(nb_seconds);
        end.Add_nsec(nanoseconds);
        duration = end - start;
    }

    // **********************************************************
    time_t Timer::Get_Duration_Seconds()
    /**
    * Returns Clock's elapsed duration in seconds (integer representation).
    */
    {
        return duration.Get_sec();
    }

    // **********************************************************
    long Timer::Get_Duration_NanoSeconds()
    /**
    * Returns Clock's elapsed duration in nanoseconds (integer representation).
    */
    {
        return duration.Get_nsec();
    }

    // **********************************************************
    double Timer::Get_Duration()
    /**
    * Returns Clock's elapsed duration in seconds (float representation).
    */
    {
        return double(Get_Duration_Seconds()) + double(Get_Duration_NanoSeconds()) / double(timing::TenToNine);
    }

    // **********************************************************
    double Timer::Calculate_Duration()
    /**
    * Get actual time and return number of seconds (float representation) since start of Clock.
    */
    {
        assert(is_initialized);

        Clock now;
        now.Get_Current_Time();
        duration = now - start;
        return Get_Duration();
    }

    // **********************************************************
    uint64_t Timer::Duration_Years()
    /**
    * Return how many years.
    */
    {
        return uint64_t(std::floor(Get_Duration() / double(timing::years_to_sec)));
    }

    // **********************************************************
    uint64_t Timer::Duration_Days()
    /**
    * Return how many months (not including full years).
    */
    {
        const uint64_t years = Duration_Years();
        double remaining_seconds = std::max(0.0, Get_Duration() - double(years*timing::years_to_sec));
        return uint64_t(std::floor(remaining_seconds / double(timing::days_to_sec)));
    }

    // **********************************************************
    uint64_t Timer::Duration_Hours()
    /**
    * Return how many hours (not including full years or days).
    */
    {
        const uint64_t years = Duration_Years();
        const uint64_t days  = Duration_Days();
        double remaining_seconds = std::max(0.0, Get_Duration() - double(years*timing::years_to_sec) - double(days*timing::days_to_sec));
        return uint64_t(std::floor(remaining_seconds / double(timing::hours_to_sec)));
    }

    // **********************************************************
    uint64_t Timer::Duration_Minutes()
    /**
    * Return how many minutes (not including full years, days or hours).
    */
    {
        const uint64_t years = Duration_Years();
        const uint64_t days  = Duration_Days();
        const uint64_t hours = Duration_Hours();
        double remaining_seconds = std::max(0.0, Get_Duration() - double(years*timing::years_to_sec) - double(days*timing::days_to_sec) - double(hours*timing::hours_to_sec));
        return uint64_t(std::floor(remaining_seconds / double(timing::min_to_sec)));
    }

    // **********************************************************
    uint64_t Timer::Duration_Seconds()
    /**
    * Return how many minutes (not including full years, days, hours or minutes).
    */
    {
        const uint64_t years    = Duration_Years();
        const uint64_t days     = Duration_Days();
        const uint64_t hours    = Duration_Hours();
        const uint64_t minutes  = Duration_Hours();
        double remaining_seconds = std::max(0.0, Get_Duration() - double(years*timing::years_to_sec) - double(days*timing::days_to_sec) - double(hours*timing::hours_to_sec) - double(minutes*timing::min_to_sec));
        return uint64_t(std::floor(remaining_seconds));
    }

    // **********************************************************
    std::string Timer::Duration_Human_Readable()
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
            duration_string += timing::IntToStr(days) + std::string("d");
        if (hours != 0 or days != 0)
            duration_string += timing::IntToStr(hours,2,'0') + std::string("h");
        if (minutes != 0 or hours != 0 or days != 0)
            duration_string += timing::IntToStr(minutes,2,'0') + std::string("m");
        duration_string += timing::IntToStr(seconds,2,'0') + std::string("s");

        return duration_string;
    }

    // **********************************************************
    void Timer::Print() const
    {
        log("Timer::Print()   %p\n", (void *)this);
        log("  Start:\n");
        start.Print();
        log("  End:\n");
        end.Print();
        log("  Duration:\n");
        duration.Print();
    }

    // **********************************************************
    void Eta::Init(const double _first_time, const double _duration)
    {
        first_time       = _first_time;
        duration         = _duration;
        Timer_Total_Ptr = &timing::TimersMap["Total"];
    }

    // **********************************************************
    std::string Eta::_Get_ETA(const double time) const
    {
        std::string eta_string("");

        // Wait 0.5% before calculating an ETA to let the simulation stabilize.
        if ((time - first_time)/duration < 5.0e-3)
        {
            eta_string = "-";
        }
        else
        {
            assert(Timer_Total_Ptr != NULL);

            // ETA: Estimated Time of Arrival (s)
            const double eta = std::max(0.0, (duration - first_time) * Timer_Total_Ptr->Calculate_Duration() / (time - first_time) - Timer_Total_Ptr->Calculate_Duration());

            Timer tmp;
            tmp.Add_Seconds(eta);

            eta_string = tmp.Duration_Human_Readable();
        }

        return eta_string;
    }

    // **********************************************************
    void Wait(const double seconds)
    /**
     * Wait for a specific amount of time.
     * NOTE: The "Timer" class will NOT count this sleep time in its Clock!
     */
    {
        timespec to_wait, remaining;
        to_wait.tv_sec = time_t(seconds);
        to_wait.tv_nsec = long((seconds - double(to_wait.tv_sec)) * timing::sec_to_nanosec);
        int return_value = nanosleep(&to_wait, &remaining);
        if (return_value != 0)
        {
            log("ERROR in Wait()\n");
            abort();
        }
    }

    // **********************************************************
    void _Print(const double nt)
    {
        const std::string s("                   ");
        bool total_found = false;
        std::string total_key;

        for (std::map<std::string, Timer>::iterator it = TimersMap.begin() ; it != TimersMap.end(); ++it )
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
        for (std::map<std::string, Timer>::iterator it = TimersMap.begin() ; it != TimersMap.end(); ++it )
        {
            if (it->first != total_key)
            {
                log("%s| %-25s | %10.5g | %13.6g | ", s.c_str(), it->first.c_str(), TimersMap[it->first].Get_Duration(), TimersMap[it->first].Get_Duration() / nt);
                    if (total_found)
                    {
                        log("%10.2f |\n", (TimersMap[it->first].Get_Duration() / TimersMap[total_key].Get_Duration())*100.0);
                    }
                    else
                        log("     -     |\n");
            }
        }
        if (total_found)
        {
            log("%s| %-25s | %10.5g | %13.6g | %10.2f |\n", s.c_str(), total_key.c_str(), TimersMap[total_key].Get_Duration(),
                                                            TimersMap[total_key].Get_Duration() / nt,
                                                            (TimersMap[total_key].Get_Duration() / TimersMap[total_key].Get_Duration())*100.0);
        }
        log("%s|---------------------------|-----------------------------------------|\n", s.c_str());
        log("%s| Total (human readable):   | %39s |\n", s.c_str(), TimersMap[total_key].Duration_Human_Readable().c_str());
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
} // namespace timing

// ********** End of file ***************************************
