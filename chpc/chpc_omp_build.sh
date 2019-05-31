#!/bin/bash

module add chpc/cmake/3.11.0/gcc-6.1.0
module add blas/gcc/64/3.6.0
module add lapack/gcc/64/3.6.0
module add chpc/pgi/19.4
module add chpc/openmpi/4.0.1/pgi/19.4

cd omp_build
cmake -DCHPC=ON -DCOMP=ON -DCMAKE_C_COMPILER="/apps/compilers/pgi/linux86-64/19.4/bin/pgcc" -DCMAKE_CXX_COMPILER="/apps/compilers/pgi/linux86-64/19.4/bin/pgc++" ../..
make