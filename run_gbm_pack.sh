declare -a layoutArray=("binbfs" "bindfs" "binstatdfs" "binstatblock")
#declare -a layoutArray=("binbfs")

MODEL_FILE="/data9/old_gbm/xgb_HIGGS.json"
DATA_FILE="/data9/HIGGS.csv"

for val in ${layoutArray[@]}; do
	PACK_FILE="/data9/packedmodel${val}.bin"
	META_FILE="/data9/metadata${val}.txt"
	./exe  --mode pack --format binary  --metadatafilename $META_FILE --layout ${val} --intertwine 2 --packfilename $PACK_FILE --modelfilename $MODEL_FILE --datafilename $DATA_FILE --numthreads 1 --package xgboost --algorithm gradientboost --task classification
	done

' :
META_FILE="/data9/gbm_HIGGSmeta.txt"
val="bfs"
PACK_FILE="/data9/packedmodelHIGGS.bin"
MODEL_FILE="/data9/xgb_HIGGS.json"
DATA_FILE="/data9/HIGGS.csv"
#MODEL_FILE="/home/ubuntu/pacset/models/xgb_model.json"
#DATA_FILE="/home/ubuntu/pacset/data/iris.csv"
./exe  --mode pack --format binary  --metadatafilename $META_FILE --layout bfs --intertwine 0 --modelfilename $MODEL_FILE --packfilename $PACK_FILE --datafilename $DATA_FILE --numthreads 1 --package xgboost --algorithm gradientboost --task classification

./exe  --mode inference --format binary  --metadatafilename $META_FILE --layout bfs --intertwine 0 --modelfilename $PACK_FILE --datafilename $DATA_FILE --numthreads 1 --package xgboost --algorithm gradientboost --task classification

./exe  --mode pack --format binary  --metadatafilename $META_FILE --layout dfs --intertwine 0 --modelfilename $MODEL_FILE --packfilename $PACK_FILE --datafilename $DATA_FILE --numthreads 1 --package xgboost --algorithm gradientboost --task classification

./exe  --mode inference --format binary  --metadatafilename $META_FILE --layout dfs --intertwine 0 --modelfilename $PACK_FILE --datafilename $DATA_FILE --numthreads 1 --package xgboost --algorithm gradientboost --task classification


./exe  --mode pack --format binary  --metadatafilename $META_FILE --layout dfsstat --intertwine 0 --modelfilename $MODEL_FILE --packfilename $PACK_FILE --datafilename $DATA_FILE --numthreads 1 --package xgboost --algorithm gradientboost --task classification

./exe  --mode inference --format binary  --metadatafilename $META_FILE --layout dfsstat --intertwine 0 --modelfilename $PACK_FILE --datafilename $DATA_FILE --numthreads 1 --package xgboost --algorithm gradientboost --task classification

./exe  --mode pack --format binary  --metadatafilename $META_FILE --layout blockstat --intertwine 0 --modelfilename $MODEL_FILE --packfilename $PACK_FILE --datafilename $DATA_FILE --numthreads 1 --package xgboost --algorithm gradientboost --task classification

./exe  --mode inference --format binary  --metadatafilename $META_FILE --layout blockstat --intertwine 0 --modelfilename $PACK_FILE --datafilename $DATA_FILE --numthreads 1 --package xgboost --algorithm gradientboost --task classification
'
