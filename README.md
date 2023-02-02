# SSD-Read-Degen-Test
Simple test program for Linux to detect SSD Read Speed Degeneration Problems

## Problem description
Some SSD have been reported to suffer from read speed degeneration: as files age the read speed can dramatically dropp (e.g. from GB/s to MB/s). This is attributed to leaks in FLASH memory cells which require the SSD controller to performe error correction and multible reads. 

This problem was originally reported some years ago (e.g. Samsung 840) and was subsequently fixed by the SSD manufatures on firmware level by automatically refreshing old data -- at the cost of a reduction the SSD lifetime.

Unfortunately this problem seems to reappear im modern NVME SSDs as FLASH memory durability seems to decrease in recent devices. 

Popular SSD performance test tools are unable to detect this phenomena because they operate on freshly created files.

There is a shareware tool for Windows to test for this phenomena, whereas no such tool seems to exist for Linux. Therefore I wrote my own tool -- not as shiny as the Windows tool but fully capable of detecting the problem (I hope). At the moment it is command line tool that recordes the result in a text file. The entries are sorted by file age (young -> old). 

## How this programm works
This programm reads files using low level I/O and calculates the read speed. It records age, file size and read speed for each file. A minimum file size can be specified to aviod artefacts when reading small files. The list of files to be tested can be specified by options passed to GNU find programm.

## Prerequisites
- GNU find (or find that suppots -print0 option)
- gcc/g++ and cmake for building the executable

## Building the programm
Run the build script in the source directory:

    ./build.sh
This will place the executable **degentest** in the project directory.

## Runnig the programm
Usage information output by the programm:

    $ ./degentest --help
    degentest [options]
    Options:
    -b | --brief          report errors only
    -f | --fopt           options to find command (default '-xdev -type f')
    -h | --help           Print this help
    -v | --verbose        verbose output
    -V | --version        Print the version number
    -m | --minsize <size> discard smaller files
                          use size suffices 'cbkMG' as defined by GNU find programm
    -o | --output <fnam>  write output to file (default stdout)
    -p | --path           Path to start at, can be called multipe times

    usage example:
    degentest-v -p / -p /home  -m 1M -o res.dat -f "-xdev -type f -maxdepth 1"
    scan directory tree / and /home, test only file  >1MB, write output to res.dat
    call find with options "-xdev -type f -maxdepth 1"

## Output File Format
The output file is a simple ASCII file the file format is as follows:

    # device_id      inode_number    Age_in_Seconds   Size_in_Bytes  Time           MB_per_Second
           41          1930962           80049            1451      1.2390e-06      1.1169e+03
        66305               28          119819        14150988      1.3628e-03      9.9029e+03
Note: The first file is too small -- typical read speed on this system is ~2.5GB/s for large files. The high read speed (~10GB/s) of the second file was caused by cashing -- the file was on purpose read twice to demonstrate the effect. 


## Grafical Display of the Results
For graphical display gnuplot or a spreadsheet have to be used. Since read speed will vary for individual files plotting an average read speed versus file age (e.g. in steps of months) should give a reliable trend.

## Interpreting the results
Variations of read speeds are to be exspected in a multitasking environment. File caching can cause unrealistic high read speeds for some files. Concurring processes can cause low read speeds -- test on a idle system or from a live system. If the average read speed drops with increasing file age a read speed degeneration is likely.

## Fixing the Read Speed Degeneration
If a firmware update is not provide by the manufacturer re-writing old files on a regular basis has to be performed by the user. This is outside of the scope of this programm.