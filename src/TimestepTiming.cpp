#include "Timing.hpp"

#include <limits>  // http://www.cplusplus.com/reference/std/limits/numeric_limits/


namespace timing
{
    // This is a timer that keeps track of the total running time.
    // The constructor starts it automatically.
    // This is needed for ETA calculation.
    extern Timer TimerTotal;

    // **********************************************************
    TimestepTiming::TimestepTiming()
    {
        elapsed_time    = 0.0;
        prev_t          = -1;
        prev_duration   = 0.0;
        nb_timesteps    = 0;
        TimerBetweenTimesteps.Start();
    }

    // **********************************************************
    void TimestepTiming::Update(const uint64_t t)
    {
        if (prev_t < t)
        {
            TimerBetweenTimesteps.Update_Duration();
            const double prev_duration_tmp = TimerBetweenTimesteps.Get_Duration();
            elapsed_time = prev_duration_tmp - prev_duration;
            prev_duration = prev_duration_tmp;
            nb_timesteps = t - prev_t;
        }
        prev_t = t;
    }

    // **********************************************************
    double TimestepTiming::_Timesteps_per_Second(const uint64_t t)
    {
        Update(t);

        if (nb_timesteps == 0 || std::abs(elapsed_time) < 1.0e-5)
            return 0.0;
        else
            return double(nb_timesteps) / elapsed_time;
    }

    // **********************************************************
    double TimestepTiming::_Seconds_per_Timestep(const uint64_t t, const double time)
    {
        return 1.0 / _Timesteps_per_Second(t);
    }

} // namespace timing

// ********** End of file ***************************************
