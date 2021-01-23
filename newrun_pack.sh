declare -a layoutArray=("binbfs" "bindfs" "binstatdfs" "binstatblock")
#declare -a layoutArray=("binbfs")

MODEL_FILE="/data6/rf_2048c.json"
DATA_FILE="/data6/c.csv"

for val in ${layoutArray[@]}; do
	PACK_FILE="/data6/packedmodel${val}.bin"
	META_FILE="/data6/metadata${val}.txt"
	./exe  --mode pack --logdir 'logs/' --labelcol 10 --format binary  --metadatafilename $META_FILE --layout $val --intertwine 2 --packfilename $PACK_FILE --modelfilename $MODEL_FILE --datafilename $DATA_FILE --numthreads 1 --package sklearn --algorithm randomforest --task regression 
	done
