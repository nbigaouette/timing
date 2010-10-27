#ifndef _INC_TIMING_hpp
#define _INC_TIMING_hpp

#include <map>
#include <string>

#include <cstdio>
#include <iostream>
#include <cmath>

#include <sys/time.h>
typedef timeval Timeval;

template <class Double>
class Timing
{
    private:
        Timeval time_info;      // Time information
        Double  duration_sec;   // Duration [seconds]
        Double  duration_usec;  // Duration [microseconds]

    public:
        // ******************************************************
        Timing(){
            Reset_Timer();
            Reset_Duration();
        }

        // ******************************************************
        void Reset_Duration()
        {
            duration_sec  = Double(0.0);
            duration_usec = Double(0.0);
            Reset_Timer();
        }

        // ******************************************************
        void Reset_Timer()
        {
            gettimeofday(&time_info, 0);
        }

        // ******************************************************
        void Get_Elapsed_Time(Double &_duration_sec, Double &_duration_usec)
        {
            // Update current time information
            timeval now;
            gettimeofday(&now, 0);

            // Store values
            const time_t now_sec  = now.tv_sec;
            const time_t now_usec = now.tv_usec;
            const time_t pre_sec  = time_info.tv_sec;
            const time_t pre_usec = time_info.tv_usec;

            // Store new time information for latter use
            gettimeofday(&time_info, 0);

            // To prevent overflow when used with floats, do the subtraction
            // separately for the seconds and the microseconds
            _duration_sec  = Double(now_sec  - pre_sec);
            _duration_usec = Double(now_usec - pre_usec);
        }

        // ******************************************************
        void Update_Duration()
        {
            const Double old_duration_sec  = duration_sec;
            const Double old_duration_usec = duration_usec;
            Get_Elapsed_Time(duration_sec, duration_usec);
            duration_sec  += old_duration_sec;
            duration_usec += old_duration_usec;
        }

        // ******************************************************
        void Update_Duration(Double elapsed_sec)
        {
            duration_sec += elapsed_sec;
        }

        // ******************************************************
        Double Get_Duration()
        {
            return (duration_sec + duration_usec*Double(1.0e-6));
        }
};

// Timing global variable
#ifdef FLOATTYPE_SINGLE
extern std::map<std::string, Timing<float> >  Timings;
#else  // #ifdef FLOATTYPE_SINGLE
extern std::map<std::string, Timing<double> > Timings;
#endif // #ifdef FLOATTYPE_SINGLE

// **************************************************************
template <class Double>
std::string Get_Elapsed_Time_String(const Double seconds)
{
    std::string time("");
    char buffer[200];

    const Double min2sec  = 60.0;
    const Double hour2min = 60.0;
    const Double day2hour = 24.0;

    Double secs_remaining = seconds;

    const int nb_days = int(secs_remaining / (day2hour * hour2min * min2sec));
    secs_remaining -= Double(nb_days) * (day2hour * hour2min * min2sec);

    const int nb_hours = int(secs_remaining / (hour2min * min2sec));
    secs_remaining -= Double(nb_hours) * (hour2min * min2sec);

    const int nb_mins = int(secs_remaining / (min2sec));
    secs_remaining -= Double(nb_mins) * (min2sec);

    const int nb_secs = int(secs_remaining);

    //sprintf(buffer, "%d days, %dh%.02d:%.02ds", nb_days, nb_hours, nb_mins, nb_secs);
    if (nb_days != 0)
    {
        sprintf(buffer, "%d days, ", nb_days);
        time += std::string(buffer);
    }
    if (nb_hours != 0 || nb_days != 0)
    {
        sprintf(buffer, "%dh", nb_hours);
        time += std::string(buffer);
    }
    if (nb_mins != 0 || nb_days != 0 || nb_hours != 0)
    {
        sprintf(buffer, "%d:", nb_mins);
        time += std::string(buffer);

        sprintf(buffer, "%.02ds", nb_secs);
    }
    else
        sprintf(buffer, "%ds", nb_secs);

    time += std::string(buffer);

    return time;
}

#endif // #ifndef _INC_TIMING_hpp

// ********** End of file ***************************************
