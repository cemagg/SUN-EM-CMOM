#!/bin/bash

#cd omp_build/
#cmake -DOpMP=ON ..
#make
cd build
export OMP_NUM_THREADS=2
time ./mom ../examples/dgfm_big/dgfm_big.mom --dgfm
