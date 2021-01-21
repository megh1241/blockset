declare -a layoutArray=("binbfs" "bindfs" "binstatdfs" "binstatblock")
#declare -a layoutArray=("binbfs")

MODEL_FILE="/data4/rf_682cifar-10.json"
DATA_FILE="/data4/cifar-10.csv"

for val in ${layoutArray[@]}; do
	PACK_FILE="/data5/packedmodel${val}.bin"
	META_FILE="/data5/metadata${val}.txt"
	./exe  --mode pack --logdir 'logs/' --labelcol 10 --format binary  --metadatafilename $META_FILE --layout ${val} --intertwine 4 --packfilename $PACK_FILE --modelfilename $MODEL_FILE --datafilename $DATA_FILE --numthreads 1 --package sklearn --algorithm randomforest --task classification
	done
