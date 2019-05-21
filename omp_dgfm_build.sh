#!/bin/bash

cd omp_build/
cmake -DOpMP=ON ..
make
export OMP_NUM_THREADS=2
time ./mom ../examples/dgfm_small/dgfm_small.mom --dgfm