#! /bin/bash

make all-parallel

list=( 1 2 4 8 12 16 20 24 28 32 )
for i in "${list[@]}"
do
	echo "running on $i processes"
	time make run-all-parallel NP=$i > /dev/null
done
