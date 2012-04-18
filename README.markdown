# timing
A simple C++ library for timing calculation using Linux's high resolution
(nanosecond) timer clock_gettime().

Instead of polluting your code with what is required to calculate timing,
this library allows you to have zero setup. Just (compile the library and)
include "Timing.hpp" anywhere you need a timer. Then, in you code, simply
start a timer:

``` C++
    TIMER_START("Timer name", TimerObject);
```
and stop it:

``` C++
    TIMER_STOP("Timer name", TimerObject);
```
See below for exact usage and meaning of the arguments.

When the calculation is done, information on the timers can be printed in
a nice formatted table using:

``` C++
    timing::Print(number_of_time_steps);
```

When calling timing::Print(), every timer created using TIMER_START() will
be printed. Information includes duration, duration per time step and ratio
with respect to total duration.


# Compilation
Optional dependency: [https://github.com/nbigaouette/stdcout](stdcout) to save
everything printed to screen in a file.

Without stdcout, please comment out these lines in the Makefile:

``` Make
# Do we want to use stdcout.git? If not, comment the following lines:
CFLAGS          += -DUSE_STDCOUT
$(eval $(call Flags_template,stdcout,StdCout.hpp,ssh://optimusprime.selfip.net/git/nicolas/stdcout.git))
```

Compilation is as follow:

``` bash
$ make clean gcc static shared install
```
"static" target builds a static library (.a) and "shared" a shared library (.so). See
https://github.com/nbigaouette/scientific_makefiles for more information on the makefiles.

By default, installation will go in ${HOME}/usr. You can install somewhere else
using DESTDIR:

``` bash
$ make gcc install DESTIR=/usr
```


# Example

Note(s):

* When linking this library with your own code, make sure to link with librt too
  (using "-lrt" LDFLAG). librt provides the timing function used "clock_gettime()"
  and comes with glibc, so any linux distribution should work without problem.
* A class "Eta" can help you calculate the "Estimated Time of Arrival" (ETA)
  of a running program. Calling its Get_ETA() member function returns a string
  containing the ETA.

``` bash
$ cat timing_test.cpp
```

``` C++
#include <Timing.hpp>

int main()
{
    // Show some nice git informations about the working directory.
    timing::Log_Git_Info();

    // This will create a new timing object called "Total".
    TIMER_START("Total", Timing_Total);

    // Do heavy calculation here
    const int max_t = 100000000;

    // Starting time is 0.0, end time is 3*max_t (two rounds of cosines and one of sines)
    static timing::Eta ETA = timing::Eta(0.0, double(3*max_t));

    // This will create a new timing object called "cosine".
    TIMER_START("cosine", Timing_Cosine);
    for (int t = 0 ; t < max_t ; t++)
    {
        const double tmp = std::cos(t);

        // Print only 10 ETA, else we are flooded...
        if (t % (max_t/10) == 0)
            std::cout << "ETA: " << ETA.Get_ETA(double(t)) << "\n";
    }
    TIMER_STOP("cosine", Timing_Cosine);

    // This will create a new timing object called "sine".
    TIMER_START("sine", Timing_Sine);
    for (int t = 0 ; t < max_t ; t++)
    {
        const double tmp = std::sin(t);

        // Print only 10 ETA, else we are flooded...
        if (t % (max_t/10) == 0)
            std::cout << "ETA: " << ETA.Get_ETA(double(max_t + t)) << "\n";
    }
    TIMER_STOP("sine", Timing_Sine);

    // Do another round of cosines. Note that the second
    // argument must be different.
    TIMER_START("cosine", Timing_Cosine2);
    for (int t = 0 ; t < max_t ; t++)
    {
        const double tmp = std::cos(t);

        // Print only 10 ETA, else we are flooded...
        if (t % (max_t/10) == 0)
            std::cout << "ETA: " << ETA.Get_ETA(double(max_t + max_t + t)) << "\n";
    }
    TIMER_STOP("cosine", Timing_Cosine2);

    TIMER_STOP("Total", Timing_Total);

    timing::Print(max_t);

    return EXIT_SUCCESS;
}

```

``` bash
$ g++ -lrt timing_test.cpp -o timing_test
$ ./timing_test
##############################################################
# Git information:                                           #
#    Library name: timing                                    #
#    Branch:       documentation                             #
#    Commit id:    991ed1de3b80b9f5325878d74ed74eab5453a6c8  #
#    Build time:   Tue Apr 17 19:13:43 EDT 2012              #
##############################################################

ETA: -
ETA: 16s
ETA: 16s
ETA: 15s
ETA: 15s
ETA: 14s
ETA: 14s
ETA: 13s
ETA: 12s
ETA: 12s
ETA: 11s
ETA: 11s
ETA: 10s
ETA: 10s
ETA: 09s
ETA: 08s
ETA: 08s
ETA: 07s
ETA: 07s
ETA: 06s
ETA: 06s
ETA: 05s
ETA: 04s
ETA: 04s
ETA: 03s
ETA: 03s
ETA: 02s
ETA: 01s
ETA: 01s
ETA: 00s
                                ____________________________________________________________________
                                |                 Timing of different code aspects                 |
                                |------------------------------------------------------------------|
                                |      Code Aspect       |          Duration          | Percentage |
                                |                        |  seconds   | per time step | over total |
                                |------------------------|------------|---------------|------------|
                                | cosine                 |     6.2799 |    6.2799e-08 |      34.33 |
                                | sine                   |     6.1386 |   6.13859e-08 |      33.56 |
                                | Total                  |     18.292 |   1.82923e-07 |      -     |
                                |------------------------|-----------------------------------------|
                                | Total (human readable) |                                     18s |
                                |------------------------------------------------------------------|


Ending time and date:
    Tuesday, April 17th 2012, 19h14:02 (20120417191402)
```

# Detailed usage

``` C++
TIMER_START("sine", Timing_Sine);
TIMER_STOP("sine", Timing_Sine);
```

TIMER_START() and TIMER_STOP() are macros. For each TIMER_START() there must be a matching
TIMER_STOP() with the exact same parameters.

The first argument to the macros is a string identifying the timer. This
is the name that will appear in timing::Print()'s output.
The second argument is the name a timer will have. Note that this name
_must_ be unique to a TIMER_START() and TIMER_STOP() pair since TIMER_START()
expands to:

``` C++
static timing::Timer &Timer_name = timing::New_Timer(name);
Timer_name.Reset();
```

The *static* attribute of the timer allows a function containing timers
to be re-called later on and increment the same timer.

Note that the second argument gives "Timer_name", so if multiple calls
of TIMER_START() are performed with the same second argument, multiple
declarations of the timer will be done and will result in a redeclaration error.
So make sure the timer names are unique. The first argument does not have to be
unique. Actually, this is how the timers are identified. See the example.

You can either use the macros or an equivalent of their expansion. The macros
are useful to simplify what gets included in the main program.


# License

This code is distributed under the terms of the [GNU General Public License v3 (GPLv3)](http://www.gnu.org/licenses/gpl.html) and is Copyright 2012 Nicolas Bigaouette.

