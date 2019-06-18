#!/bin/bash

cd build_mpi/
cmake -DMPI=ON ..
make
export OMP_PROC_BIND=true
export OMP_PLACES=cores
time mpiexec -np 1 -x OMP_NUM_THREADS=2 ./mom_mpi ../examples/bow_tie_array/bow_tie_array.mom --dgfm
# time mpiexec -np 4 -x OMP_NUM_THREADS=4 ./mom_mpi ../examples/bow_tie_array/bow_tie_array_m.mom --dgfm