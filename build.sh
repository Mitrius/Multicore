#!/bin/bash
source /opt/intel/bin/compilervars.sh intel64
#sequential
g++ -O3 -std=c++14 -Wall -Wextra sources/* -W -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -fopenmp

#parallel
#g++ -std=c++11 -g -Wall -Wextra sources/* -W -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm -ldl
