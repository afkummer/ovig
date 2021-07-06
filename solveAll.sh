#!/bin/bash

for f in new-instances/*; do
   printf "Solving %s...\n" $f
   ./solve.sh $f 1 | tee output.log
   ./parseLog.py output.log
   rm -f output.log
   printf "\n\n\n"
done

