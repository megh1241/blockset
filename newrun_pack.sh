#declare -a layoutArray=("binbfs" "bindfs" "binstatdfs" "binstatblock")
declare -a layoutArray=("bfs")

MODEL_FILE="/data5/rf_20HIGGS.json"
DATA_FILE="/data5/HIGGS.csv"

for val in ${layoutArray[@]}; do
	PACK_FILE="/data5/packedmodel${val}.bin"
	META_FILE="/data5/metadata${val}.txt"
	./exe  --mode pack --logdir 'logs/' --labelcol 10 --format binary  --metadatafilename $META_FILE --layout ${val} --intertwine 1 --packfilename $PACK_FILE --modelfilename $MODEL_FILE --datafilename $DATA_FILE --numthreads 1 --package sklearn --algorithm gradientboost --task classification
	done
