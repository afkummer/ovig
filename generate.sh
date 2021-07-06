#!/bin/bash

mkdir new-instances 2> /dev/null

if [ $# -ne 2 ]; then
   echo "Usage: $0 <1:num patients> <2:num caregivers>"
   exit 1
fi

n=$1
v=$2

# You can change the number of instances to generate by modifying the
# argument for the `seq` command.
for i in `seq 10`; do
   printf "Generating instance #%s...\n" $i
   dest="HHCRSP_${n}_${v}_${i}.txt"

   # Create the profile usin GNU SED.
   sed "s/XXXX/${dest}/g; s/ZZZZ/${n}/g; s/YYYY/${v}/g; s/AAAA/$i/g" template-sed.conf > profile.conf

   # Generates the instance
   ./main -c profile.conf -s $i

   # Moves the new instance to the destination dir.
   echo Moving data to new-instances/$dest
   mv daily_0.txt new-instances/$dest
   rm -rf profile.conf

   printf "\n\n\n"
done
