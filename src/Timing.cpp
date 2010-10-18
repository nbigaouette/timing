
#include <cstdio>
#include <iostream>
#include <cmath>

#include "Timing.hpp"

// Timing global variable
std::map<std::string, Timing> Timings;

std::string Get_Elapsed_Time_String(const fdouble seconds)
{
    std::string time("");
    char buffer[200];

    const fdouble min2sec  = 60.0;
    const fdouble hour2min = 60.0;
    const fdouble day2hour = 24.0;

    fdouble secs_remaining = seconds;

    const int nb_days = int(secs_remaining / (day2hour * hour2min * min2sec));
    secs_remaining -= fdouble(nb_days) * (day2hour * hour2min * min2sec);

    const int nb_hours = int(secs_remaining / (hour2min * min2sec));
    secs_remaining -= fdouble(nb_hours) * (hour2min * min2sec);

    const int nb_mins = int(secs_remaining / (min2sec));
    secs_remaining -= fdouble(nb_mins) * (min2sec);

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

// **************************************************************
Timing::Timing()
{
    Reset_Timer();
    Reset_Duration();
}

// **************************************************************
void Timing::Reset_Duration()
{
    duration = 0.0;
    Reset_Timer();
}

// **************************************************************
void Timing::Reset_Timer()
{
    gettimeofday(&time_info, 0);
}

// **************************************************************
fdouble Timing::Get_Elapsed_Time()
{
    fdouble sec = 0.0;
    timeval now;
    gettimeofday(&now, 0);
    const time_t now_sec  = now.tv_sec;
    const time_t now_usec = now.tv_usec;
    const time_t pre_sec  = time_info.tv_sec;
    const time_t pre_usec = time_info.tv_usec;
    gettimeofday(&time_info, 0);
    // To prevent overflow when used with floats, do the subtraction
    // separatly for the seconds and the microseconds
    sec = fdouble(now_sec - pre_sec) + (fdouble(now_usec - pre_usec) / fdouble(1.0e6));
    return sec;
}

// **************************************************************
void Timing::Update_Duration()
{
    duration += Get_Elapsed_Time();
}

// **************************************************************
void Timing::Update_Duration(fdouble elapsed_sec)
{
    duration += elapsed_sec;
}

// **************************************************************
fdouble Timing::Get_Duration()
{
    return duration;
}


// ********** End of file ***************************************

