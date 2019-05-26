#!/bin/bash

module add chpc/cmake/3.11.0/gcc-6.1.0
module add blas/gcc/64/3.6.0
module add lapack/gcc/64/3.6.0
module add chpc/pgi/19.4

cd build
cmake -DCHPC=ON -DCMAKE_C_COMPILER="/apps/compilers/pgi/linux86-64/19.4/bin/pgcc" -DCMAKE_CXX_COMPILER="/apps/compilers/pgi/linux86-64/19.4/bin/pgc++" ..
make
