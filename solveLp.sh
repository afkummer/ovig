#!/bin/bash

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
mkdir solverlog 2>&1 > /dev/null 2> /dev/null

bn="$(basename $inst .txt)"
logLp="$(mktemp -p solverlog logLp.$bn.$seed.XXXXXXXX.log)"

/home/alberto/work/doct-ufrgs-thesis/mankowska2014/build/exactMip $inst 999999 4 0 | tee $logLp

exit $?

