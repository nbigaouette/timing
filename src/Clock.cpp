
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
        // "CLOCK_PROCESS_CPUTIME_ID" measure the time taken by the process on the CPU.
        // If the process is sleeping (using timing::Wait() for example), the time passed
        // sleeping will not be counted!
        //const int return_value = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &(this->timer));
        // "CLOCK_REALTIME" might have trouble in 2038 and is affected by clock changes (NTP)
        //const int return_value = clock_gettime(CLOCK_REALTIME, &(this->timer));
        // "CLOCK_MONOTONIC" should be the most reliable clock
        const int return_value = clock_gettime(CLOCK_MONOTONIC, &(this->timer));
        if (return_value != 0)
        {
            log("ERROR: Failed calling clock_gettime()\n");
            abort();
        }
    }

    // **********************************************************
    void Clock::Print() const
    {
        log("    sec:  %ld\n", timer.tv_sec);
        log("    nsec: %ld\n", timer.tv_nsec);
    }
} // namespace timing

// ********** End of file ***************************************