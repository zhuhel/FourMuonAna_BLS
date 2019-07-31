if [ $# -ge 1 ]; then 
  INPUT=$1
else 
  INPUT="Runlist/input_test.txt"
fi

if [ ! -f ${INPUT} ]; then eosmount ~/eos; fi

outdir=`basename ${INPUT}`

if [ $# -ge 2 ]; then 
  OUTPUT=$2
else 
  OUTPUT="output/${outdir}"
fi

if [ -d ${OUTPUT} ]; then
  rm -rf ${OUTPUT}
fi

if [ ! -d `dirname ${OUTPUT}` ]; then
  mkdir `dirname ${OUTPUT}` -p
fi

echo "testllqq -outdir $OUTPUT -in $INPUT"
runFourMuon -outdir $OUTPUT -in $INPUT
#runFourMuon2 -outdir $OUTPUT -in $INPUT
