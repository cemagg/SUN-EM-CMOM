#!/bin/bash

cd abuild
cmake -DOpMP=ON ..
make
export OMP_NUM_THREADS=4
time ./mom ../examples/bow_tie_array/bow_tie_array_med.mom --dgfm