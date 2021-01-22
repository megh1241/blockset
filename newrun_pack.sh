declare -a layoutArray=("bfs" "dfs" "statdfs" "statblock")
#declare -a layoutArray=("binbfs")

MODEL_FILE="/data5/rf_628HIGGS.json"
DATA_FILE="/data5/test_HIGGS.csv"

for val in ${layoutArray[@]}; do
	PACK_FILE="/data5/packedmodel${val}.bin"
	META_FILE="/data5/metadata${val}.txt"
	./exe  --mode pack --logdir '/data5/logs/' --labelcol 10 --format binary  --metadatafilename $META_FILE --layout ${val} --intertwine 0 --packfilename $PACK_FILE --modelfilename $MODEL_FILE --datafilename $DATA_FILE --numthreads 1 --package sklearn --algorithm gradientboost --task classification
	done
