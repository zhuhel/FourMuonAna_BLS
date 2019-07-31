#!/bin/bash

DataDir="/lustre/umt3/datadisk/mc16_13TeV/DAOD_BPHY4"

for dir in `ls $DataDir`;
do
  listname0=`basename ${dirs}`
  list=${listname0}.list

  for file in `ls ${dirs}/*root*`; do

   echo $dir
done 
#for file in ${DataDir}/*.root*
#do
#  echo $file
#done
