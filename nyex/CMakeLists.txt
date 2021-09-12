cmake_minimum_required(VERSION 3.19)
project(nyex3 )

set(CMAKE_C_STANDARD 17 )
add_compile_options(-Wc++11-extensions)
#set_target_properties(nyex3 PROPERTIES LINKER_LANGUAGE CXX)
add_definitions(-I/home/eladmin/anaconda3/lib/python3.8/site-packages/numpy/core/include
        -I/home/eladmin/anaconda3/include/python3.8 -I/usr/local/include)
add_executable(nyex3
#        nyex.c nyex-main-py3.cpp rolling_window.cpp rolling_window.h nyex-welford-py3.cpp
        skew_kurt.cpp)
