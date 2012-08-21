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
        prev_t          = std::numeric_limits<uint64_t>::max();
        nb_timesteps    = 0;
    }

    // **********************************************************
    void TimestepTiming::Update(const uint64_t t, const double time)
    {
        if (t < prev_t)
        {
            TimerBetweenTimesteps.Update_Duration();
            elapsed_time = TimerTotal.Get_Duration();
            nb_timesteps = t - prev_t;
        }
    }

    // **********************************************************
    double TimestepTiming::_Timesteps_per_Second(const uint64_t t, const double time)
    {
        Update(t, time);

        return double(nb_timesteps) / elapsed_time;
    }

    // **********************************************************
    double TimestepTiming::_Seconds_per_Timestep(const uint64_t t, const double time)
    {
        return 1.0 / _Timesteps_per_Second(t, time);
    }

} // namespace timing

// ********** End of file ***************************************
