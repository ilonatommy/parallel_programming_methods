#!/bin/bash
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=01:00:00
#SBATCH --partition=plgrid
#SBATCH --account=plgilonat2020a
module add plgrid/tools/openmpi

mpicc -o pi_par pi_par.c
for ((i=1; i<=12; i++)); do
        mpiexec -np $i ./pi_par 100000000
        declare -i n=$(( 100000000 * i))
	mpiexec -np $i ./pi_par $n 
done
