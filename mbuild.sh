#!/bin/bash

cd build_mpi/
cmake -DMPI=ON ..
make
mpiexec -np 4 ./mom_mpi ../examples/example-8/bow_tie_array.mom --dgfm