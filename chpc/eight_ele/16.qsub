#!/bin/bash
#PBS -l select=1:ncpus=24:mpiprocs=1:nodetype=haswell_reg
#PBS -P CSCI1214
#PBS -q serial
#PBS -l walltime=10:00:00
#PBS -o /mnt/lustre/users/tebrahim/SUN-EM-CMOM/chpc/results/init_test/eight_1p_4t.out
#PBS -e /mnt/lustre/users/tebrahim/SUN-EM-CMOM/chpc/results/init_test/eight_1p_4t.err
#PBS -m abe
#PBS -M 17650739@sun.ac.za
#PBS -N eight_1p_4t

ulimit -s unlimited

nproc= cat $PBS_NODEFILE | wc -l

cd /mnt/lustre/users/tebrahim/SUN-EM-CMOM/chpc/ompi_build

module add chpc/openblas/0.2.19/gcc-6.1.0
module add chpc/openmpi/4.0.0/gcc/6.1.0

export OPENBLAS_NUM_THREADS=4
export GOMP_CPU_AFFINITY="0 1 2 3"
time mpiexec -np 1 -machinefile $PBS_NODEFILE -x OMP_NUM_THREADS=4 mom_mpi ../../examples/bow_tie_array/bow_tie_array_m.mom --dgfm
