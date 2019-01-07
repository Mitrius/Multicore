#!/bin/bash
#source /opt/intel/bin/compilervars.sh intel64

g++ -std=c++11 -g -Wall reference.cpp -W -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -ldl -lm
