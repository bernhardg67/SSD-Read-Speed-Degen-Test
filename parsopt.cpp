#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "defaults.h"
#include "parsopt.h"
#include "usage.h"
#include "version.h"

// long options
static struct option long_options[] = {{"brief", no_argument, 0, 'b'},
                                       {"fopt", required_argument, 0, 'f'},
                                       {"help", no_argument, 0, 'h'},
                                       {"minsize", required_argument, 0, 'm'},
                                       {"output", required_argument, 0, 'o'},
                                       {"path", required_argument, 0, 'p'},
                                       {"verbose", no_argument, 0, 'v'},
                                       {"version", no_argument, 0, 'V'},
                                       {0, 0, 0, 0}};

// short options
char short_options[] = "f:hm:o:p:bvV";

// function prototype in this file
size_t getsize(char *optarg);

void parse_commandline(int argc, char **argv) {
    int c;
    std::vector<std::string> opt_find_dirtrees;

    // extract filename from FQPN
    std::string prgname = std::filesystem::path(argv[0]).filename();

    while (1) {
        /* ‘getopt_long’ stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, short_options, long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {
        case 'b': // brief: non verbose
            verbose_flag = false;
            break;

        case 'f': // fopt: options to find command
            find_options = optarg;
            break;

        case 'h': // help
            usage(prgname);
            exit(EXIT_SUCCESS);
            break;

        case 'p': // path: append to start path for find
            opt_find_dirtrees.push_back(optarg);
            break;

        case 'm': // minsize: discard smaller files
            min_size = getsize(optarg);
            break;

        case 'o': // output: write output to file
            result_fnam = optarg;
            break;

        case 'v': // verbose
            verbose_flag = true;
            break;

        case 'V': // version
            std::cout << prgname << " " << VERSION_STRING << std::endl;
            exit(EXIT_SUCCESS);
            break;

        case '?':
            /* ‘getopt_long’ already printed an error message. */
            usage(prgname);
            exit(EXIT_FAILURE);
            break;

        default:
            std::cout << "option " << (char)c
                      << " not implemented. Blame the programmer." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Print any remaining command line arguments (not options) and terminate. 
    if (optind < argc) {
        std::cout << "Too many command line arguments given:" << std::endl;;
        while (optind < argc)
            std::cout <<  argv[optind++] << " ";
        std::cout << std::endl;
        exit(EXIT_FAILURE);
    }

    // if option was given overwrite default tree
    if (!opt_find_dirtrees.empty())
        find_dirtrees = opt_find_dirtrees;
}

// get size using find like suffixes cbkMG
size_t getsize(char *optarg) {
    int lastchar = strlen(optarg) - 1;
    char sc = optarg[lastchar];
    long unit = 0;
    switch (sc) {
    case 'c':
        unit = 1;
        break;
    case 'b':
        unit = 1 << 9;
        break;
    case 'k':
        unit = 1 << 10;
        break;
    case 'M':
        unit = 1 << 20;
        break;
    case 'G':
        unit = 1 << 30;
        break;
    default:
        std::cout << "Invalid or no size spezifier given. Give me: cbkMG." << std::endl;
        exit(EXIT_FAILURE);
    };

    optarg[lastchar] = 0;
    long sz = atol(optarg) * unit;
    if (sz <= 0) {
        std::cout << "Invalid minimumg size given: " << sz << std::endl;
        exit(EXIT_FAILURE);
    }

    return sz;
}
