# CMoM - A C++ Method of Moments Solver

### <u>Dependencies</u>

- CMake
- MPI Library (OpenMPI or MPICH)
- OpenMP

## Optional
- Python 3

### Serial Install Guide

1. mkdir build
2. cd build
3. cmake ..
4. make
5. ./mom -h

### Parallel Install Guide

1. mkdir build_mpi
2. cd build_mpi
3. cmake -DMPI=ON ..
4. make
5. ./mom_mpi -h

### Usage from Gui

