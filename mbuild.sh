#!/bin/bash

cd build_mpi/
cmake -DMPI=ON ..
make
mpiexec -np 4 ./mom_mpi ../examples/two_plate_array/two_plate_array_coarse.mom