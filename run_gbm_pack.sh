META_FILE="/data9/gbm_meta.txt"
val="bfs"
PACK_FILE="/data9/packedmodel.bin"
#MODEL_FILE="/data9/cifar2048_xgb.json"
#DATA_FILE="/data9/cifar-10.csv"
MODEL_FILE="/home/ubuntu/pacset/models/xgb_model.json"
DATA_FILE="/home/ubuntu/pacset/data/iris.csv"

./exe  --mode pack --format binary  --metadatafilename $META_FILE --layout $val --intertwine 0 --modelfilename $MODEL_FILE --packfilename $PACK_FILE --datafilename $DATA_FILE --numthreads 1 --package xgboost --algorithm gradientboost --task classification

./exe  --mode inference --format binary  --metadatafilename $META_FILE --layout $val --intertwine 0 --modelfilename $PACK_FILE --datafilename $DATA_FILE --numthreads 1 --package xgboost --algorithm gradientboost --task classification
