cmake_minimum_required(VERSION 3.16)
project(symbreg_dx)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_C_COMPILER /usr/local/Cellar/gcc/11.2.0/bin/gcc-11)
#set(CMAKE_CXX_COMPILER /usr/local/Cellar/gcc/11.2.0/bin/g++-11)

#add_executable(symbreg_dx main.cpp)


#file(GLOB SYMBREG_SRC  *.cpp *.h Utils/*.cpp)
file(GLOB SYMBREG_SRC  main.cpp *.h Utils/*.cpp)
#file(GLOB SYMBREG_DATA examples/symbreg/*.conf examples/symbreg/ReadMe.txt)
link_directories(/usr/local/lib /usr/lib/x86_64-linux-gnu)

add_executable(symbreg ${SYMBREG_SRC} Table.cpp  Utils/rolling_window.cpp Utils/rolling_window.h)
#add_executable(symbreg ${SYMBREG_SRC}   Utils/rolling_window.cpp Utils/rolling_window.h)
#add_compile_options(-ffast-math -ffloat-store -O3 )
#add_compile_options(-O0)
#add_dependencies(symbreg openbeagle-GP openbeagle-GA openbeagle arrow)
find_package(NumCpp 2.1.0 REQUIRED)
target_link_libraries(symbreg openbeagle-GP openbeagle-GA openbeagle pacc tbb  NumCpp::NumCpp arrow parquet)
#set_target_properties(symbreg PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BEAGLE_OUTPUT_DIR}/bin/symbreg")
#MoveConfigFiles("${SYMBREG_DATA}" "${BEAGLE_OUTPUT_DIR}/bin/symbreg" false)
#install(TARGETS symbreg DESTINATION bin/openbeagle/symbreg)
#install(FILES ${SYMBREG_DATA} DESTINATION bin/openbeagle/symbreg)
