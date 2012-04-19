
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
#include <sys/stat.h> // Check if folder exists

namespace timing
{
    // **********************************************************
    // Variables global to the library but hidden from program

    // Map dynamically containing all timers
    std::map<std::string, Timer> TimersMap;
    // This is a timer that keeps track of the total running time.
    // The constructor starts it automatically.
    // This is needed for ETA calculation.
    Timer TimerTotal;
    // Flags to enable/disable timing information output
    std::string output_folder;  // Directory where to save timing information
    uint64_t    timers_step;    // Current time step

    // **********************************************************
    // Local to this file function declarations
    void Create_Folder_If_Does_Not_Exists(const std::string path);

    // **********************************************************
    Timer & New_Timer(const std::string &_name)
    {
        Timer &new_timer = TimersMap[_name];
        new_timer.Set_Name(_name);
        return new_timer;
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
    void Stop_All_Timers()
    {
        for (std::map<std::string, Timer>::iterator it = TimersMap.begin() ; it != TimersMap.end() ; ++it)
        {
            it->second.Stop();
        }

        // Set total timer's name manually
        TimerTotal.Set_Name("Total");

        // Total timer's duration clock is updated at each time step. Clear it
        // because Stop() increments the duration using "duration = duration + (end - start)"
        TimerTotal.duration.Clear();

        TimerTotal.Stop();
    }

    // **********************************************************
    void Print_N_Times(const std::string x, const size_t N, const bool newline)
    {
        for (size_t i = 0 ; i < N ; i++)
        {
            log("%s", x.c_str());
        }

        if (newline)
            log("\n");
    }

    // **********************************************************
    void Print_Code_Aspect(const std::string &s,
                           const Timer &timer,
                           const std::string &timer_name,
                           const size_t longest_length,
                           const uint64_t nt)
    {
        std::string timer_name_w_spaces("");
        timer_name_w_spaces = timer_name;
        timer_name_w_spaces.resize(longest_length, ' ');

        log("%s|", s.c_str());
        log(" %s ", timer_name_w_spaces.c_str());
        log("| %10.5g | %13.6g | ", timer.Get_Duration(), timer.Get_Duration() / double(nt));

        log("%10.2f |\n", (timer.Get_Duration() / TimerTotal.Get_Duration())*100.0);
    }

    // **********************************************************
    void _Print(const uint64_t nt)
    /**
     *
     *  @param  nt  Number of time steps (iterations) done in the main program.
     */
    {
        Stop_All_Timers();

        size_t longest_length = 0;
        size_t current_length = 0;
        for (std::map<std::string, Timer>::iterator it = TimersMap.begin() ; it != TimersMap.end() ; ++it)
        {
            // Find longest name
            current_length = it->first.length();
            if (current_length > longest_length)
            {
                longest_length = current_length;
            }
        }

        std::string total_human_readable("Total (human readable)");
        const std::string timings("Timing of different code aspects");
        longest_length = std::max(longest_length, total_human_readable.length());
        const size_t total_length_minus_longest = 44;
        const size_t total_length = total_length_minus_longest + longest_length; // Does not include the first and last "|"

        // Center the table inside 128 columns
        const size_t length_max = 128;
        const size_t length_s = size_t(std::floor(double(length_max - total_length+2) / 2.0));
        const std::string s(length_s, ' ');

        log("%s", s.c_str());
        Print_N_Times("_", total_length+2);

        {
            const size_t length = timings.length();
            const size_t length_left  = (total_length - length) / 2;
            const size_t length_right =  total_length - length - length_left;
            log("%s|", s.c_str());
            Print_N_Times(" ", length_left, false);
            log("%s", timings.c_str());
            Print_N_Times(" ", length_right, false);
            log("|\n");
        }

        log("%s|", s.c_str());
        Print_N_Times("-", total_length, false);
        log("|\n");

        {
            const std::string code_aspects("Code Aspect");
            const size_t length = code_aspects.length();
            const size_t length_left  = (longest_length+2 - length) / 2;
            const size_t length_right =  longest_length+2 - length - length_left;
            log("%s|", s.c_str());
            Print_N_Times(" ", length_left, false);
            log("%s", code_aspects.c_str());
            Print_N_Times(" ", length_right, false);
            log("|");
        }
        log("          Duration          | Percentage |\n");

        log("%s|", s.c_str());
        Print_N_Times(" ", longest_length+2, false);
        log("|  seconds   | per time step | over total |\n");

        log("%s|", s.c_str());
        Print_N_Times("-", longest_length+2, false);
        log("|------------|---------------|------------|\n");

        for (std::map<std::string, Timer>::iterator it = TimersMap.begin() ; it != TimersMap.end(); ++it )
        {
            Print_Code_Aspect(s, TimersMap[it->first], it->first, longest_length, nt);
        }

        log("%s|", s.c_str());
        Print_N_Times("-", longest_length+2, false);
        log("|------------|---------------|------------|\n");

        // Print total last
        Print_Code_Aspect(s, TimerTotal, "Total", longest_length, nt);

        log("%s|", s.c_str());
        Print_N_Times("-", longest_length+2, false);
        log("|-----------------------------------------|\n");

        total_human_readable.resize(longest_length, ' ');
        log("%s| %s | %39s |\n", s.c_str(), total_human_readable.c_str(), TimerTotal.Duration_Human_Readable().c_str());

        log("%s|", s.c_str());
        Print_N_Times("-", total_length, false);
        log("|\n\n");

        time_t rawtime;
        time(&rawtime);
        const int timing_max_string_size = 1000;
        char date_out[timing_max_string_size];      // Output string
        struct tm *date_format; // Saves in Date format
        date_format = localtime(&rawtime);
        strftime(date_out, timing_max_string_size, "%A, %B %dth %Y, %Hh%M:%S (%Y%m%d%H%M%S)", date_format);
        log("\nEnding time and date:\n    %s\n", date_out);
    }

    // **************************************************************
    void Create_Folder_If_Does_Not_Exists(const std::string path)
    {
        struct stat statBuf;
        if (stat(path.c_str(), &statBuf) < 0)
        {
            std::string cmd = "mkdir -p " + path;
            const int return_value = system(cmd.c_str());
            if (return_value != 0)
            {
                log("ERROR: Folder '%s' could not be created correclty!\n");
                log("       Disabling timing output.\n");
                output_folder = "";
            }
        }
    }

    // **********************************************************
    void Enable_Timers_Output(const std::string &_output_folder)
    /**
     * Store a non-empty string as the output folder to enable saving
     * of the timing information to a file.
     */
    {
    }

    // **********************************************************
    void Set_Timers_Step(const uint64_t _step)
    /**
     * If saving timing information is desired, set the current time step.
     */
    {
    }
} // namespace timing

// ********** End of file ***************************************
