#!/bin/bash

cd build_mpi/
cmake -DMPI=ON ..
make
#time mpiexec -np 1 -x OMP_NUM_THREADS=1 ./mom_mpi ../examples/bow_tie_array/bow_tie_array.mom --dgfm
time mpiexec -np 4 ./mom_mpi ../examples/bow_tie_array/bow_tie_array.mom --dgfm