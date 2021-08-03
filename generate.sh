#!/bin/bash

echo "${@}"

mkdir new-instances 2> /dev/null

if [ $# -ne 5 ]; then
   echo "Usage: $0 <1:num patients> <2:num caregivers> <3:density> <4:distribution> <5:depot placement>"
   exit 1
fi

n=$1
v=$2
dens=$3
dist=$4
dplace=$5

case "${dist}" in
   "cluster")
      dist_lbl="C"
   ;;

   "cluster-random")
      dist_lbl="RC"
   ;;

   "random")
      dist_lbl="R"
   ;;
esac

case "${dplace}" in
   "central")
      dplace_lbl="C"
   ;;

   "random")
      dplace_lbl="R"
   ;;
esac

# You can change the number of instances to generate by modifying the
# argument for the `seq` command.
for i in `seq 100`; do
   printf "Generating instance #%s...\n" $i
   dest="HHCRSP_${n}_${v}_${i}_${dens}_${dplace_lbl}_${dist_lbl}.txt"

   # Create the profile usin GNU SED.
   sed "s/XXXX/${dest}/g; s/ZZZZ/${n}/g; s/YYYY/${v}/g; s/AAAA/$i/g; s/BBBB/$dens/g; s/CCCC/$dist/g; s/DDDD/$dplace/g" template-sed.conf > profile.conf

   # Generates the instance
   ./main -c profile.conf -s $i

   # Moves the new instance to the destination dir.
   echo Moving data to new-instances/$dest
   mv daily_0.txt new-instances/$dest
   rm -rf profile.conf

   if [[ ! -f "metad.csv" ]]; then
      echo "instance,patients,caregivers,seed,depot,distribution,dens" > "metad.csv"
   fi
   echo "new-instances/${dest},${n},${v},${i},${dplace_lbl},${dist_lbl},${dens}" >> "metad.csv"

   printf "\n\n\n"
done


