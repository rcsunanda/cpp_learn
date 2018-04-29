#!/bin/bash

if [ "$#" == 0 ]; then
    make
    scp clock_benchmark_* octo1@172.25.34.11:~/sunandag/clock_benchmark
    exit 0
fi

if [ $1 = "cl" ]; then 
    make clean
    make
    scp clock_benchmark_* octo1@172.25.34.11:~/sunandag/clock_benchmark
    exit 0
fi

if [ $1 = "cmake" ]; then
    find ! -name 'compile.sh' -type f -exec rm -f {} +
    cmake ../ -DCMAKE_BUILD_TYPE=Release
    make
    scp clock_benchmark_* octo1@172.25.34.11:~/sunandag/clock_benchmark
    exit 0
fi

echo "Invalid arguments"

