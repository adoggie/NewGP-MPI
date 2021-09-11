cmake_minimum_required(VERSION 3.19)
project(nyex C)

set(CMAKE_C_STANDARD 11)
add_definitions(-I/home/eladmin/anaconda3/lib/python3.8/site-packages/numpy/core/include
        -I/home/eladmin/anaconda3/include/python3.8 -I/usr/local/include)
add_executable(nyex
        nyex.c nyex-main-py3.cpp rolling_window.cpp rolling_window.h nyex-welford-py3.cpp)
