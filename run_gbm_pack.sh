META_FILE="/home/ubuntu/pacset/gbm_meta.txt"
val="bfs"
PACK_FILE="/home/ubuntu/pacset/packedmodel.bin"
MODEL_FILE="/home/ubuntu/pacset/models/final_xgb.json"
DATA_FILE="/home/ubuntu/pacset/data/iris.csv"

./exe  --mode pack --format binary  --metadatafilename $META_FILE --layout $val --intertwine 0 --modelfilename $MODEL_FILE --packfilename $PACK_FILE --datafilename $DATA_FILE --numthreads 1 --package xgboost --algorithm gradientboost --task classification

./exe  --mode inference --format binary  --metadatafilename $META_FILE --layout $val --intertwine 0 --modelfilename $PACK_FILE --datafilename $DATA_FILE --numthreads 1 --package xgboost --algorithm gradientboost --task classification
