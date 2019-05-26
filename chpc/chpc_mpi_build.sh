#!/bin/bash

module add chpc/cmake/3.11.0/gcc-6.1.0
module add blas/gcc/64/3.6.0
module add lapack/gcc/64/3.6.0
module add chpc/openmpi/4.0.1/pgi/19.4

cd mpi_build
cmake -DCHPC=ON -DMPI=ON -DCMAKE_C_COMPILER="/apps/libs/openmpi/4.0.1-pgi19.4/bin/mpicc" -DCMAKE_CXX_COMPILER="/apps/libs/openmpi/4.0.1-pgi19.4/bin/mpic++" ../..
make