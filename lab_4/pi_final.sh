#!/bin/bash
SIZE=(10000000 1000000000)
mpicc -o pi_par pi_par.c

for s in ${SIZE[@]}; do
	for ((i=1; i<=12; i++)); do
        	declare -i n=$(( s * i))
		mpiexec -machinefile ./allnodes -np $i ./pi_par $n >>output.txt	 
	done
done

