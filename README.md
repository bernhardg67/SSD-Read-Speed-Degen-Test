# SSD-Read-Degen-Test
Simple test program for Linux to detect SSD Read Speed Degeneration Problems

## Problem description
Some SSD have been reported to suffer from read speed degeneration: as files age the read speed can dramatically dropp (e.g. from GB/s to MB/s). This is attributed to leaks in FLASH memory cells which require the SSD controller to perform error correction and multible reads. 

This phenomena was originally reported some years ago (e.g. Samsung 840) and was subsequently fixed by the SSD manufatures on firmware level by automatically refreshing old data -- at the cost of a reduction the SSD lifetime.

Unfortunately this penomena reappers in modern NVME SSDs as FLASH memory durability seems to decrease in recent devices.

Popular SSD performance test tools are unable to detect this phenomena because they operate on freshly created files.

There is a Freeware tool for Windows to test for this phenomena, whereas no such tool exists for Linux. Therefore I wrote my own tool -- not as shiny as the Windows tool but fully capable of detecting the problem (I hope). At the moment it is a command line tool that recordes the result in a text file or stdout. The entries are sorted by file age (young -> old). 

## How this Program Works
This program reads files using low level I/O and calculates the read speed. It records age, file size and read speed for each file. A minimum file size can be specified to aviod artefacts when reading small files. The list of files to be tested can be specified by options passed to GNU find program.

## Prerequisites
- GNU find (or find that suppots -print0 option)
- gcc/g++ and cmake for building the executable

## Building the program
Run the build script in the source directory:

    ./build.sh
This will place the executable **degentest** in the project directory.

## Runnig the program
Usage information output by the program:

    $ ./degentest --help
    degentest [options]
    Options:
    -b | --brief          report errors only
    -f | --fopt           options to find command (default '-xdev -type f')
    -h | --help           Print this help
    -v | --verbose        verbose output
    -V | --version        Print the version number
    -m | --minsize <size> discard smaller files
                          use size suffices 'cbkMG' as defined by GNU find program
    -o | --output <fnam>  write output to file (default stdout)
    -p | --path           Path to start at, can be called multipe times

    usage example:
    degentest-v -p / -p /home  -m 1M -o res.dat -f "-xdev -type f -maxdepth 1"
    scan directory tree / and /home, test only file  >1MB, write output to res.dat
    call find with options "-xdev -type f -maxdepth 1"

## Output File Format
The output file is a simple ASCII file. The file format is as follows:

    # device_id      inode_number    Age_in_days      Size_in_MB    Time            MB_per_Second
     00:1a            1836310             0.0             1.1      1.39e-04            8193.7
     00:1a             424229            40.1             1.4      1.92e-03             742.9
Note: Typical a read speed on this system is ~2.5GB/s to ~ ~3.5GB/s for large, recently created files. The first file shows artificially high read speed (~8GB/s) due to cashing effects. The second file has a suspiciously low read speed. Since many files of similar age also showed up with read speed <1GB/s this is an indication for my SSD suffering from Read Rate Degeneration. 

To identify the affected file you can use find like this:

    $ find / -inum 424229 -exec ls -l {} \; 2>/dev/null
    -rwxr-xr-x 1 root root 1495296 22. Aug 2020  /usr/bin/lftp


## Limitations 
There is no reliable method to determine the **true** file age (= time of last write to the file). The mtime (time of last content modification) can be set to any time and this is typically done for installed software (e.g. stuff in /usr, see the example above with /usr/bin/lftp which was installed in 12/2022). These files may be younger as indicated by mtime. Most filesystems support btime (birthtime, time of file creation). If btime is younger than mtime, the program uses btime, otherwise mtime. This should solve the problem in **most** cases. However if mtime is set to some time in after the last write operation we are doomed. The program may even report negative file ages, since mtime can be set to the future. This is not a bug!

## Grafical Display of the Results
For graphical display e.g. gnuplot or a spreadsheet progam may to be used. Since read speed will vary for individual files plotting an average read speed versus file age (e.g. in steps of weeks or months) should show a reliable trend.

## Interpreting the results
Variations of read speed is to be exspected in a multitasking environment. File caching can cause unrealistic high read speed for some files. Concurring processes can cause low read speed -- run the tool on a idle system or from a live system. If the average read speed drops with increasing file age read speed degeneration is likely.

## Fixing the Read Speed Degeneration
If a firmware update is not provide by the manufacturer re-writing old files on a regular basis has to be performed by the user.