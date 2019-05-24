#!/bin/bash

cd abuild
cmake -DOpMP=ON ..
make
export OMP_NUM_THREADS=2
time ./mom ../examples/dgfm_big/dgfm_big.mom --dgfm
