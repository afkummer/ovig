#!/bin/bash

# Precisa ajustar conforme o caminho que está compilado
# o código da heurística.
SOLVER_PATH=/home/alberto/work/itor-2021-brkga-mp-ipr-hhcrsp/build/brkga

if [ $# -lt 1 ]; then
   echo "Usage: $0 <1:instance path> [2:seed]"
   echo "The seed is optional, and set by default to 1."
   exit 1
fi

inst=$1
seed=1

# To prevent concurrency while writing logs and CSV files, we first store 
# all outputs, and then use a parse script to extract the important data 
# from the outputs.
mkdir solverlog 2>&1 > /dev/null

bn="$(basename $inst .txt)"
logGa="$(mktemp -p solverlog logGa.$bn.$seed.XXXXXXXX.log)"
logLp="$(mktemp -p solverlog logLp.$bn.$seed.XXXXXXXX.log)"


if [ $# -eq 2 ]; then
   seed=$2
fi

# $SOLVER_PATH \
#    --popsize 599 \
#    --gens 636 \
#    --pe 0.33384 \
#    --pm 0.0805 \
#    --mp 15 \
#    --ep 11 \
#    --bp linear \
#    --npop 2 \
#    --npairs 1 \
#    --mindist 0.60391 \
#    --psel best \
#    --pperc 0.01808 \
#    --ptype permutation \
#    --pperiod 68 \
#    --reset 52 \
#    --xelite 8 \
#    --immigrants 2 -i $inst -s $seed

# -gens 700
$SOLVER_PATH \
   --popsize 205 \
   --gens 448 \
   --pe 0.31835 \
   --pm 0.06329 \
   --mp 3 \
   --ep  2 \
   --bp constant \
   --npop 1 \
   --npairs 1 \
   --mindist 0.4429 \
   --psel best \
   --pperc 0.58126 \
   --ptype permutation \
   --pperiod 96 \
   --reset 68 \
   --xelite 22 \
   --immigrants 8\
   -i $inst \
   -s $seed | tee $logGa &

if [ $? -ne 0 ]; then
   echo $1 >> problematic.txt
   exit 1
fi

/home/alberto/work/doct-ufrgs-thesis/mankowska2014/build/exactMip $inst 999999 4 0 | tee $logLp

echo "Waiting for everything to finish."
fg
echo "The waiting ended."

exit $?

