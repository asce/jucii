#!/bin/bash

rm 1024_4.tmp 2048_4.tmp 4096_4.tmp 8192_4.tmp

echo "Creando 1024_4.tmp ...";
for((i = 0;i < 5;i++)) 
do
mpirun -c 4 julia -p 1024 1024 >> 1024_4.tmp;
done

echo "Creando 2048_4.tmp ...";
for((i = 0;i < 5;i++))
do
mpirun -c 4 julia -p 2048 2048 >> 2048_4.tmp;
done

echo "Creando 4096_4.tmp ...";
for((i = 0;i < 5;i++))
do
mpirun -c 4 julia -p 4096 4096 >> 4096_4.tmp;
done

echo "Creando 8192_4.tmp ...";
for((i = 0;i < 5;i++))
do
mpirun -c 4 julia -p 8192 8192 >> 8192_4.tmp;
done
