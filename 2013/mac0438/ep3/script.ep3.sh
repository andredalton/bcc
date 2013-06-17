#!/bin/bash

mkdir -p ./graficos

HV='1 50 100 150 200'
BV='1 10 30'
NV='1 50 100 150 200'
TV='1 40 200'
tV='1 10 100'

for H in $HV; do
   for B in $BV; do
      for N in $NV; do
         for T in $TV; do
            for t in $tV; do
               python ./ep3.py $N $B $H $t $T -g >/dev/null;
               echo -e "\nOK $N $B $H $t $T\n";
            done
         done
      done
   done
done
