
#include "Timing.hpp"

// See https://github.com/nbigaouette/stdcout
#ifdef USE_STDCOUT
// If stdcout.git is wanted, include it.
#include <StdCout.hpp>
#else
// If stdcout.git is not wanted, define log() as being printf().
#define log printf
#endif // #ifdef USE_STDCOUT

#include <cstdlib>
#include <cstring> // memset()

namespace timing
{
    extern std::map<std::string, Timer> TimersMap;

    // This is a timer that keeps track of the total running time.
    // The constructor starts it automatically.
    // This is needed for ETA calculation.
    extern Timer TimerTotal;
    // Flags to enable/disable timing information output
    extern std::string output_folder;  // Directory where to save timing information
    extern uint64_t    timers_step;    // Current time step

    // **********************************************************
    void Timer::Set_Name(const std::string &_full_name, const std::string &_strict_name)
    {
        name = _full_name;

        // Only set the filename once. Since Set_Name() can be called multiple
        // times for the same _full_name but with a different _strict_name,
        // only set the filename once.
        if (output_filename == "")
            output_filename = output_folder + "/" + _strict_name + ".csv";
    }

    // **********************************************************
    Timer::Timer()
    /**
     * Default constructor.
     */
    {
        Clear();
        Start();
    }

    // **********************************************************
    Timer::Timer(const Timer &other)
    /**
     * Copy constructor. Required so that Timer class can contain
     * an std::ofstream (output_file).
     */
    {
        is_started      = other.is_started;
        start           = other.start;
        end             = other.end;
        duration        = other.duration;
        name            = other.name;
        output_filename = other.output_filename;
        counter         = other.counter;
    }

    // **********************************************************
    void Timer::Clear()
    {
        is_started = false;
        counter = 0;
        start.Clear();
        end.Clear();
        current_duration.Clear();
        duration.Clear();
    }

    // **********************************************************
    void Timer::Start()
    {
        if (not is_started)
        {
            ++counter;
            output_has_been_performed = false;
            start.Get_Current_Time();
        }
        is_started = true;
    }

    // **********************************************************
    void Timer::Stop()
    {
        if (is_started)
        {
            is_started = false;

            end.Get_Current_Time();
            current_duration = end - start;
            duration = duration + current_duration;
        }

        // Save timing information
        if (not output_has_been_performed and output_folder != "")
        {
            if (output_filename != "")
            {
                //log("Saving timer's output to \"%s\".\n", output_filename.c_str());

                // If first write, try to open file.
                if (not output_file.is_open())
                {
                    output_file.open(output_filename.c_str(), std::ios_base::out);

                    // Try to add a header
                    if (not output_file.is_open())
                        log("ERROR: Could not open file \"%s\"!\n", output_filename.c_str());
                    else
                    {
                        output_file << "#    Step,               Start            , Duration\n";
                    }
                }

                // File should be opened now. Attempt write.
                if (not output_file.is_open())
                    log("ERROR: Could not open file \"%s\"!\n", output_filename.c_str());
                else
                {
                    output_file << std::setw(9) << timers_step << ", " << start.Get_Time() << ", " << Get_Current_Duration() << "\n";
                }
            }
            else
            {
                log("ERROR: Timer's filename is empty!\n");
                Print();
                abort();
            }

            output_has_been_performed = true;
        }
    }

    // **********************************************************
    uint64_t Timer::Get_Counter() const
    {
        return counter;
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
    time_t Timer::Get_Duration_Seconds() const
    /**
    * Returns Clock's elapsed duration in seconds (integer representation).
    */
    {
        return duration.Get_sec();
    }

    // **********************************************************
    long Timer::Get_Duration_NanoSeconds() const
    /**
    * Returns Clock's elapsed duration in nanoseconds (integer representation).
    */
    {
        return duration.Get_nsec();
    }

    // **********************************************************
    double Timer::Get_Current_Duration() const
    /**
    * Returns Clock's elapsed duration in seconds (float representation)
    * Only the current duration is returned, not the total cumulative.
    */
    {
        return double(current_duration.Get_sec()) + double(current_duration.Get_nsec()) / double(timing::TenToNine);
    }

    // **********************************************************
    double Timer::Get_Duration() const
    /**
    * Returns Clock's elapsed duration in seconds (float representation).
    */
    {
        return double(Get_Duration_Seconds()) + double(Get_Duration_NanoSeconds()) / double(timing::TenToNine);
    }

    // **********************************************************
    void Timer::Update_Duration()
    /**
    * Get actual time and return number of seconds (float representation) since start of Clock.
    * NOTE: This function does NOT increment "duration" clock, it just sets it.
    *       If the timer is stopped and restarted many times, the duration will be erased.
    */
    {
        if (is_started)
            end.Get_Current_Time();

        duration = end - start;
    }

    // **********************************************************
    uint64_t Timer::Duration_Years()
    /**
    * Return how many years.
    */
    {
        return uint64_t(std::floor(Get_Duration() / double(years_to_sec)));
    }

    // **********************************************************
    uint64_t Timer::Duration_Days()
    /**
    * Return how many months (not including full years).
    */
    {
        const uint64_t years = Duration_Years();
        double remaining_seconds = std::max(0.0, Get_Duration() - double(years*years_to_sec));
        return uint64_t(std::floor(remaining_seconds / double(days_to_sec)));
    }

    // **********************************************************
    uint64_t Timer::Duration_Hours()
    /**
    * Return how many hours (not including full years or days).
    */
    {
        const uint64_t years = Duration_Years();
        const uint64_t days  = Duration_Days();
        double remaining_seconds = std::max(0.0, Get_Duration() - double(years*years_to_sec) - double(days*days_to_sec));
        return uint64_t(std::floor(remaining_seconds / double(hours_to_sec)));
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
        double remaining_seconds = std::max(0.0, Get_Duration() - double(years*years_to_sec) - double(days*days_to_sec) - double(hours*hours_to_sec));
        return uint64_t(std::floor(remaining_seconds / double(min_to_sec)));
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
        const uint64_t minutes  = Duration_Minutes();
        double remaining_seconds = std::max(0.0, Get_Duration() - double(years*years_to_sec) - double(days*days_to_sec) - double(hours*hours_to_sec) - double(minutes*min_to_sec));
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
        const uint64_t minutes  = Duration_Minutes();
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
        log("Timer::Print() name: %s (%p)\n", name.c_str(), (void *)this);
        log("  Start:\n");
        start.Print();
        log("  End:\n");
        end.Print();
        log("  Current Duration:\n");
        current_duration.Print();
        log("  Total Duration:\n");
        duration.Print();
    }
} // namespace timing

// ********** End of file ***************************************
