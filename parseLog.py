#!/usr/bin/env python3
#! -*- coding: utf-8 -*-

# Support for reading XZ-compressed data.
import lzma

def openLog(fname:str):
   if ".xz" in fname:
      return lzma.open(fname, "rt")
   else:
      return open(fname, "rt")


if __name__ == "__main__":
   from sys import argv
   if len(argv) != 2:
      print("Usage: %s <1:logfile path>")
      exit(1)

   instance = ""
   seed = -1

   data = []

   with openLog(argv[1]) as fid:
      for line in fid:
         tks = [x.strip() for x in line.split() if len(x.strip()) > 0]
         if len(tks) > 0 and tks[0] == "Instance:":
            instance = tks[1]
         if len(tks) > 0 and tks[0] == "Seed:":
            seed = int(tks[1])
         if len(tks) >= 10 and tks[0] not in ['Gens', 'Cost', 'Skill', 'Task', 'Unused']:
            if tks[0] == '*':
               tks = tks[1:]
            data.append(tks)
         if "Unused vehicle list:" in line:
            unused = tks[-4]

   tks = [x.strip() for x in instance.split("_") if len(x.strip()) > 0]
   nodes = tks[-6]
   vehi = tks[-5]
   iseed = tks[-4]
   density = tks[-3]
   dplace = tks[-2]
   dist = tks[-1].split(".")[0]
   
   with open("results.csv", "a+") as fid:
      if fid.tell() == 0:
         fid.write("instance,nodes,vehi,iseed,density,dplace,distrib,")
         fid.write("seed,generation,remaining,cost.local,el.diver,no.impr,cost,dist,tard,tmax,time,op.flags,op.pr,op.xe,op.rst,unused.vehi\n")
      for row in [data[-1]]:
         fid.write(instance + "," + nodes + "," + vehi + "," + iseed + "," + density + "," + dplace + "," + dist + ",")
         fid.write(str(seed) + ",")
         fid.write(','.join(row))
         if 'P' not in row[-1] and 'X' not in row[-1] and 'R' not in row[-1]:
            fid.write(",,0,0,0")
         else:
            fid.write(",")
            fid.write("1," if 'P' in row[-1] else "0,")
            fid.write("1," if 'X' in row[-1] else "0,")
            fid.write("1" if 'R' in row[-1] else "0")
         fid.write("," + unused)
         fid.write("\n")

