# timing
A simple C++ template library for timing calculation.

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


``` bash
$ cat timing_test.cpp
```

``` C++
#include <Timing.hpp>
std::map<std::string, Timing<double> > Timings;

int main()
{
    Timings["Total"].Reset_Timer();

    // Do heavy calculation here
    const int max_t = 100000000;
    Timings["cos"].Reset_Timer();
    for (int t = 0 ; t < max_t ; t++)
    {
        const double tmp = std::cos(t);
    }
    Timings["cos"].Update_Duration();

    Timings["sin"].Reset_Timer();
    for (int t = 0 ; t < max_t ; t++)
    {
        const double tmp = std::sin(t);
    }
    Timings["sin"].Update_Duration();

    Timings["Total"].Update_Duration();

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
                   | cos                       |     5.5667 |   5.56667e-08 |      49.82 |
                   | sin                       |     5.6075 |   5.60753e-08 |      50.18 |
                   | Total                     |     11.174 |   1.11742e-07 |     100.00 |
                   |---------------------------|-----------------------------------------|
                   | Total (human readable):   |                                     11s |
                   |---------------------------------------------------------------------|


Ending time and date:
    Monday, February 20th 2012, 13h01:50 (20120220130150)
```

# License

This code is distributed under the terms of the [GNU General Public License v3 (GPLv3)](http://www.gnu.org/licenses/gpl.html) and is Copyright 2011 Nicolas Bigaouette.

