#!/bin/bash

cd build_mpi/
cmake -DMPI=ON ..
make
mpiexec -np 4 ./mom_mpi ../examples/pec_plate.mom