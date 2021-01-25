
val="binstatblock"
PACK_FILE="/home/ubuntu/pm"
#PACK_FILE="/home/ubuntu/pacset/packedmodelbinstatdfs.txt"
META_FILE="/home/ubuntu/pacset/metadatabinstatblock.txt"
DATA_FILE="/home/ubuntu/pacset/cifar-10.csv"
./exe  --mode inference --format binary --metadatafilename ${META_FILE} --layout $val --intertwine 3 --modelfilename $PACK_FILE --datafilename $DATA_FILE --numthreads 1 --package sklearn --algorithm randomforest --task classification
