#!/bin/bash

cd abuild
cmake -DOpMP=ON ..
make
export OMP_NUM_THREADS=4
time ./mom ../test/test_cases/cbfm_debug/cbfm_debug.mom --dgfm