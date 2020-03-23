#!/bin/bash

mpicc -o pi_par pi_par.c
for ((i=1; i<=12; i++)); do
        mpiexec -machinefile ./allnodes -np $i ./pi_par 100000000
        declare -i n=$(( 100000000 * i))
	mpiexec -machinefile ./allnodes -np $i ./pi_par $n 
done
