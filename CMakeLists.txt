cmake_minimum_required(VERSION 3.1)
project(ssd_read_deg_test)
message (STATUS "${CMAKE_BUILD_TYPE}")

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

set(TARGET degentest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(${TARGET}
    main.cpp 
    runfind.cpp 
    defaults.cpp 
    parsopt.cpp 
    timedread.cpp 
    readtest.cpp
    writeresult.cpp
    usage.cpp
    statx.cpp
    )
