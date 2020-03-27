#!/bin/bash
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --exclusive
#SBATCH --constraint="intel"
#SBATCH --time=02:00:00
#SBATCH --partition=plgrid
#SBATCH --account=plgilonat2020a
module add plgrid/tools/openmpi
I=(1 2 3)
SIZE=(10000000 1000000000 40000000000)
mpicc -o pi_par pi_par.c
for i in ${I[@]}; do
	for s in ${SIZE[@]}; do
		for ((i=1; i<=12; i++)); do
        		mpiexec -np $i ./pi_par $s >>z_out_ns.txt
        		declare -i n=$(( s * i))
                mpiexec -np $i ./pi_par $n >>z_out_sc.txt
		done
	done
done
