
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
    void Eta::Init(const double _first_time, const double _duration)
    {
        first_time       = _first_time;
        duration         = _duration;
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
            // ETA: Estimated Time of Arrival (s)
            TimerTotal.Update_Duration();
            const double elapsed_time = TimerTotal.Get_Duration();
            const double eta = std::max(0.0, ((duration - first_time) / (time - first_time) - 1.0) * elapsed_time);

            Timer tmp;
            tmp.Clear();
            tmp.Add_Seconds(eta);
            eta_string = tmp.Duration_Human_Readable();
        }

        return eta_string;
    }
} // namespace timing

// ********** End of file ***************************************
