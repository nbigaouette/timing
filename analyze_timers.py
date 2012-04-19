#!/usr/bin/env python2
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np
import sys, os, glob
import datetime as dt


# ****************************************************************************************************************************************************
# http://docs.python.org/library/optparse.html
from optparse import OptionParser
parser = OptionParser()
parser.add_option("-i", "--input", type=str,            dest="path",                   default=None,    help="Folder containing timers files [default: %default]")
parser.add_option("-t", "--type",  type=str,            dest="types", action="append", default=None,    help="Type(s) of figure(s) (possibilities: barh, ts)")
parser.add_option(      "--scale", action="store_true", dest="scale",                  default=False,   help="Scale time step plot by number of time steps [default: %default]")
(options, args) = parser.parse_args()
# ****************************************************************************************************************************************************

if (options.path == None):
    print "ERROR: Please use -i/--input to point to a folder where to load timers information."
    sys.exit(0)

if (options.types == None):
    print "ERROR: Please at least one type of figure using --t/--type."
    sys.exit(0)

#full_path = os.path.abspath(options.path)
full_path = options.path

if (not os.path.exists(full_path)):
    print "ERROR: Folder \"" + full_path + "\" does not exists!"
    sys.exit(0)

# Get a list of all timers
timer_files = glob.glob(os.path.join(full_path, "*"))
# Place "Total" timer file at top
timer_files.sort()
timer_files = timer_files[::-1]
Total_filename = os.path.join(full_path, "Timing_Total.csv")
timer_files.remove(Total_filename) # Remove it
timer_files.append(Total_filename) # Add it back at end

colors = ['blue', 'red', 'magenta']

class Timer:
    def __init__(self, timer_file):
        self.filename = timer_file
        self.name = os.path.splitext(os.path.basename(self.filename))[0].replace("Timing_", "")
        #print "timer_file =", timer_file
        #print "timer_name =", self.timer_name

        self.t = 0
        self.nb_timers = 0

        self.load_csv()

    def load_csv(self):
        self.data = np.loadtxt(self.filename, delimiter=',', skiprows=1, comments='#', converters = {1: mpl.dates.datestr2num}, ndmin=2)

        #print "data =", self.data

        self.step       = self.data[:,0]
        self.start      = self.data[:,1]
        self.duration   = self.data[:,2]

        self.nb = len(self.step)

        self.dates_start = np.array(mpl.dates.num2date(self.start))
        self.dates_end   = np.array(mpl.dates.num2date(self.start))
        self.dates_dur   = np.zeros(self.nb, dtype = dt.timedelta)
        for i in xrange(self.nb):
            self.dates_dur[i] = dt.timedelta(seconds = self.duration[i])
            self.dates_end[i] += self.dates_dur[i]

    def set_time0(self, time0):
        self.time0 = time0

    def set_order(self, t, nb_timers):
        self.t = t
        self.nb_timers = nb_timers

    def plot_hbar(self, fig, ax):
        ypos = np.arange(self.t, self.t+1)+0.5
        left = mpl.dates.date2num(self.dates_start)
        self.rects = []
        for i in xrange(self.nb):
            # Width is expected in days. Change seconds to days.
            width = self.dates_dur[i].total_seconds() / (60.0 * 60.0 * 24.0)
            # +1 in colors array to make sure that "Total" timer gets blue, even if at the end of the list
            self.rects.append(
                    ax.barh(bottom=ypos, width=width, left=left[i],
                            align='center', height=1.0, color=colors[(self.t+1) % len(colors)],
                            alpha = 0.6)
                )

            # Add timer name
            text_x = self.dates_start[i] + dt.timedelta(seconds = self.dates_dur[i].total_seconds() / 2.0)
            ax.text(text_x, ypos, self.name, horizontalalignment='center',
                    verticalalignment='center', color='black')

    def plot_timesteps(self, fig, ax):
        print "Plotting \"" + self.name + "\""
        if (len(self.step) == 1):
            plot_step = np.array([0, self.step[0]])
            plot_duration = np.array([1.0e-7, self.duration[0]])
        else:
            plot_step = self.step
            plot_duration = self.duration

        delta_ts = 1.0
        if (options.scale):
            # Scale the duration by the number of time steps span.
            delta_ts = plot_step[1::] - plot_step[0:-1:]
            delta_ts = np.concatenate((np.array([plot_step[0]]), delta_ts))

        ax.semilogy(plot_step, plot_duration / delta_ts, color = colors[(self.t+1) % len(colors)], label = self.name)



# Load timers information
timers = []
for timer_file in timer_files:
    # Append a new timer to list
    timers.append(Timer(timer_file))
nb_timers = len(timers)

# Find Total timer and set all others time0
for i in xrange(nb_timers):
    if (timers[i].name == "Total"):
        # Set all timer's starting time
        for j in xrange(nb_timers):
            timers[j].set_time0(timers[j].start[0])

for t in xrange(nb_timers):
    timers[t].set_order(t, nb_timers)

# Plot
fig = plt.figure()
ax = fig.add_subplot(111)

for figue_type in options.types:
    if (figue_type == "barh"):
        plt.setp(ax.get_yticklabels(), visible=False)
        for t in xrange(nb_timers):
            timers[t].plot_hbar(fig, ax)
        ax.xaxis_date()

        ax.xaxis.set_major_formatter(mpl.dates.DateFormatter('%H:%M:%S'))
        ax.xaxis.set_major_locator(mpl.dates.SecondLocator(interval=1))

        ax.grid(True)
    elif (figue_type == "ts"):
        for t in xrange(nb_timers):
            timers[t].plot_timesteps(fig, ax)

        ax.set_xlabel("Time step")
        ax.set_ylabel("Duration (seconds)")
        ax.legend(loc="best")

plt.show()





