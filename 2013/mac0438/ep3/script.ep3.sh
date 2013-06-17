#!/bin/bash

mkdir -p ./graficos

for H in {1..10}; do
   for B in {1..10}; do
      for N in {1..10}; do
         for T in {1..2..100}; do
            for t in {1..10}; do
               python ./ep3.py $N $B ${H} $t $T -g >/dev/null;
               echo -e "\nOK $N $B $H $t $T\n";
            done
         done
      done
   done
done
