#!/usr/bin/python
# This Python file uses the following encoding: utf-8

# Plot results from SSD Read Speed Test
# usage: plotresult <filename>
# Plots individual files, average of age class (interval of 7 days) with errorbars, total average

from matplotlib import pylab
from matplotlib import pyplot as plt
import numpy as np
import sys
from PyQt5.QtWidgets import QFileDialog, QApplication


if len(sys.argv) > 1:
    fnam = sys.argv[1]  # file to plot
else:
    app = QApplication(sys.argv)
    fnam, _ = QFileDialog.getOpenFileName(None, "Select data to plot...",
                                          "", filter="All files (*);;Data files (*.dat)")
    if fnam == "":
        sys.exit(0)


interval = 7      # number of days to average over
max_bin_num = 30  # maximum number of bin
y_axis_max = 3    # for scatter polt: y axsis max = 3 * average speed

# read data file
dtype1 = np.dtype([('age', 'f8'), ('mbps', 'f8')])
a = np.loadtxt(fnam, dtype=dtype1, skiprows=1, usecols=(0, 3))
age = np.array(a['age'])
speed = np.array(a['mbps'])
av_speed_total = np.mean(speed)  # average speed of all files

# for averaging
bin_min = age.min() * 0.99999
bin_max = age.max() * 1.00001
bin_range = bin_max - bin_min
# range 1 ... max_bin_num
nbins = round(min(max(bin_range/interval, 1), max_bin_num))
step = bin_range/nbins

# print('min_age: ', age.min())
# print('max_age: ', age.max())
# print('nuber of bins: ', nbins)
# print('interval size: ', step)

# calculate average speed for the bins
av_speed = np.array([])   # average speed for each bin
std_speed = np.array([])  # standard deviation for each bin
av_age = np.array([])
av_entries = np.array([])
for binnum in range(0, nbins):
    bin = np.where((age >= bin_min + binnum*step) &
                   (age < bin_min + (binnum+1)*step))
    entries_in_bin = bin[0].size
    if entries_in_bin > 0:
        av_entries = np.append(av_entries, entries_in_bin)
        av_age = np.append(av_age, np.mean(age[bin]))
        av_speed = np.append(av_speed, np.mean(speed[bin]))
        std_speed = np.append(std_speed, np.std(speed[bin]))

# print("av_entries", av_entries)
# print("av_age", av_age)
# print("av_rate", av_speed)
# print("av_speed_total", av_speed_total)


titlefont = {
    'color':  'black',
    'weight': 'bold',
    'size': 16,
}
infofont = {
    'color':  'black',
    'weight': 'normal',
    'size': 12,
}


fig = plt.figure(figsize=(16, 9), dpi=100)
axis = fig.add_axes([0.1, 0.1, 0.8, 0.8])

axis.set_xlabel("age [days]")
axis.set_ylabel("read speed [MB/s]")
axis.set_title("Read Speed vs File Age",  fontdict=titlefont)
axis.set_title("Number of files tested: " + str(age.size), fontdict=infofont, loc='right')
axis.scatter(age, speed, label="individual files")
axis.errorbar(av_age, av_speed, yerr=std_speed, color='r',
              label="average (age class)")  # , xlolims=True, 'r*-')
axis.axhline(y=av_speed_total, color='r',
             linestyle='--', label="average (total)")
axis.legend(loc=1)

# scatter plot: don't plot outliner
axis.set_ylim(0, min(speed.max(), 3*av_speed_total))


plt.show()
