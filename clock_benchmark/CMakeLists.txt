cmake_minimum_required (VERSION 2.8)

project (clock_benchmark)


include_directories(${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_LIST)

#separate binaries for different clock types (CLOCK_TYPE macro)

#SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
#SET(BUILD_SHARED_LIBRARIES OFF)
#SET(CMAKE_EXE_LINKER_FLAGS "-static")

set (TARGET1 clock_benchmark_posix_monotonoic)
add_executable (clock_benchmark_posix_monotonoic ${SRC_LIST})
set_target_properties (${TARGET1} PROPERTIES COMPILE_FLAGS "-DCLOCK_TYPE=1")
target_link_libraries (${TARGET1} pthread rt)

set (TARGET2 clock_benchmark_rdtscp)
add_executable (${TARGET2} ${SRC_LIST})
set_target_properties (${TARGET2} PROPERTIES COMPILE_FLAGS "-DCLOCK_TYPE=2")
target_link_libraries (${TARGET2} pthread rt)

set (TARGET3 clock_benchmark_chrono)
add_executable (${TARGET3} ${SRC_LIST})
set_target_properties (${TARGET3} PROPERTIES COMPILE_FLAGS "-DCLOCK_TYPE=3")
target_link_libraries (${TARGET3} pthread rt)

set (TARGET4 clock_benchmark_posix_realtime)
add_executable (${TARGET4} ${SRC_LIST})
set_target_properties (${TARGET4} PROPERTIES COMPILE_FLAGS "-DCLOCK_TYPE=4")
target_link_libraries (${TARGET4} pthread rt)


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -ggdb -m64 -std=c++11")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -ggdb -std=c++11 -fsanitize=address")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -m64 -std=c++11")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -std=c++11")