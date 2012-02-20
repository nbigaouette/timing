# timing
A simple C++ template library for timing calculation. By using an std::map to
store the timing objects, a new object can just be created by accessing a new
map member.

When calling Print_Timing_Info(), every member of the map will be printed.
Printed information includes duration, duration per time step and ratio with
respect to total duration.


# Compilation
Requirements: [https://github.com/nbigaouette/stdcout](stdcout).

It's just a simple header file. Either copy it to alongside the rest of the code,
or install it using:

``` bash
$ make install
```

By default, it will be installed in ${HOME}/usr. You can install somewhere else
using DESTDIR:

``` bash
$ make install DESTIR=/usr
```


# Example

Notes:

* Because of Print_Timing_Info() prints all its members, the map needs to have a
the specific name "Timings".
* A timing object "Total" must be present. This is to calculate the ration of all
different timings.
* Since the templated library can be specialized to floats or doubles, the
function Print_Timing_Info() needs to know which one to use. It will thus require
an argument of the same type as what was specified when the map is declared. Even
though the argument is a float or a double, its value is an integer that represents
the total number of time steps used in the measured calculation. This allows
printing the duration _per time step_ too.


``` bash
$ cat timing_test.cpp
```

``` C++
#include <Timing.hpp>
std::map<std::string, Timing<double> > Timings;

int main()
{
    // This will create a new timing object called "Total".
    Timings["Total"].Reset_Timer();

    // Do heavy calculation here
    const int max_t = 100000000;

    // This will create a new timing object called "cosine".
    Timings["cosine"].Reset_Timer();
    for (int t = 0 ; t < max_t ; t++)
    {
        const double tmp = std::cos(t);
    }
    Timings["cosine"].Update_Duration();

    // This will create a new timing object called "sine".
    Timings["sine"].Reset_Timer();
    for (int t = 0 ; t < max_t ; t++)
    {
        const double tmp = std::sin(t);
    }
    Timings["sine"].Update_Duration();

    Timings["Total"].Update_Duration();

    // Argument must be the same as what is used when specializing Timing objects.
    Print_Timing_Info(double(max_t));

    return EXIT_SUCCESS;
}

```

``` bash
$ g++ -I${HOME}/usr/include -L${HOME}/usr/lib -lz -lstdcout -Wl,-rpath,${HOME}/usr/lib timing_test.cpp -o timing_test
$ ./timing_test

                   _______________________________________________________________________
                   |                  Timing of different code aspects                   |
                   |---------------------------------------------------------------------|
                   |       Code Aspect         |          Duration          | Percentage |
                   |                           |  seconds   | per time step | over total |
                   |---------------------------|------------|---------------|------------|
                   | cosine                    |     5.5667 |   5.56667e-08 |      49.82 |
                   | sine                      |     5.6075 |   5.60753e-08 |      50.18 |
                   | Total                     |     11.174 |   1.11742e-07 |     100.00 |
                   |---------------------------|-----------------------------------------|
                   | Total (human readable):   |                                     11s |
                   |---------------------------------------------------------------------|


Ending time and date:
    Monday, February 20th 2012, 13h01:50 (20120220130150)
```

# License

This code is distributed under the terms of the [GNU General Public License v3 (GPLv3)](http://www.gnu.org/licenses/gpl.html) and is Copyright 2011 Nicolas Bigaouette.

