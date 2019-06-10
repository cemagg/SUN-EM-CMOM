#!/bin/bash

module add chpc/cmake/3.11.0/gcc-6.1.0
module add chpc/atlas/3.10.3/gcc-6.1.0
module add chpc/netcdf/4.4.4-F/gfortran/gfortran-6.1.0 
module add chpc/openmpi/4.0.0/gcc/6.1.0 

cd a
cmake -DCATL=ON -DMPI=ON ../..
make
