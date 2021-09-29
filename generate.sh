#!/bin/bash

# Check script arguments.
if [ $# -ne 6 ]; then
   echo "Usage: $0 <1:seed> <2:num patients> <3:num caregivers> <4:density> <5:distribution> <6:depot placement>"
   exit 1
fi

# Always create the destination directory.
mkdir new-instances 2> /dev/null

# Extract the arguments.
seed=$1
n=$2
v=$3
dens=$4
dist=$5
dplace=$6

# Parses some parameters to generate the correct instance name.
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

# Prepare the scratch data to generate the instance profile.
prof="$(mktemp -p .)"
#printf "Generating instance (seed=%s)...\n" $seed
dest="HHCRSP_${n}_${v}_${seed}_${dens}_${dplace_lbl}_${dist_lbl}.txt"

# Create the profile usin GNU SED.
sed "s/XXXX/${dest}/g; s/ZZZZ/${n}/g; s/YYYY/${v}/g; s/AAAA/$seed/g; s/BBBB/$dens/g; s/CCCC/$dist/g; s/DDDD/$dplace/g" template-sed.conf > $prof

# Generates the instance
./main -c $prof -s $seed > /dev/null

# Moves the new instance to the destination dir.
#echo Moving data to new-instances/$dest
mv $dest new-instances/$dest
rm -rf $prof

if [[ ! -f "metad.csv" ]]; then
   echo "instance,patients,caregivers,seed,depot,distribution,dens" > "metad.csv"
fi
echo "new-instances/${dest},${n},${v},${seed},${dplace_lbl},${dist_lbl},${dens}" >> "metad.csv"

#printf "\n\n\n"
