#!/bin/sh
echo "running degentest on / and /home as root."
echo "writing output to perf.dat"
sudo ./degentest -p / -p /home -m 1k -o perf.dat 