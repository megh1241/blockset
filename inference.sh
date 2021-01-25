
DATA_FILE="/home/ubuntu/cifar-10.csv"
#PACK_FILE="/home/ubuntu/packedmodelstatblock.txt"
PACK_FILE="/home/ubuntu/pm"
META_FILE="/home/ubuntu/metadatastatblock.txt"
LAYOUT="statblock"

./exe  --mode inference --format binary  --metadatafilename $META_FILE --layout $LAYOUT --intertwine 0 --modelfilename $PACK_FILE --datafilename $DATA_FILE --numthreads 1 --package sklearn --algorithm randomforest --task classification
