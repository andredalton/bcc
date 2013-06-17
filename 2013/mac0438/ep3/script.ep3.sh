#!/bin/bash

mkdir -p ./graficos

for H in {1..200..50}; do
   for B in [1 10 30]; do
      for N in {1..200..50}; do
         for T in [1 40 200]; do
            for t in [1 10 100]; do
               python ./ep3.py $N $B $H $t $T -g >/dev/null;
               echo -e "\nOK $N $B $H $t $T\n";
            done
         done
      done
   done
done
