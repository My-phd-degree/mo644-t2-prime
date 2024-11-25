#!/bin/bash
for i in $(seq 1 5) 
do
  echo "Instance $i:" 
  for j in $(seq 1 15) 
  do 
    time=$(./build/parallel tests/$i.in 2>&1 1>/dev/null > build/$i.out && diff -q build/$i.out build/$i.out.ser)
    echo "experiment $j;$time" 
  done 
done
