#!/bin/bash

#DataDir="/lustre/umt3/datadisk/mc16_13TeV/DAOD_BPHY4"
DataDir="/lustre/umt3/user/helingzh/reprocessed8TeV/"

for file in ${DataDir}/data12_8TeV*BPHY1*/*.root*
do
  echo $file
done
