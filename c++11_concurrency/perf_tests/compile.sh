g++ -std=c++11 pthread_mutex_vs_std_mutex_overhead.cpp -o mutex_overhead_westmere -lpthread -lrt -O3 -march=westmere -m64
cp mutex_overhead_westmere tsxperf1@172.25.70.210:~/test/concurrency