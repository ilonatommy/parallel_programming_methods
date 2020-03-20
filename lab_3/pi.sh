#!bin/bash -l
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --exclusive
#SBATCH --time=01:00:00
#SBATCH --partition=plgrid
#SBATCH --account=plgilonat2020a
module add plgrid/tools/openmpi
mpicc -o pi_par pi_par.c
mpiexec -np 12 ./pi_par 100000000
