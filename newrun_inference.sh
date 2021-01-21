
DATA_FILE="/data4/cifar-10.csv"

declare -a layoutArray=("binbfs" "bindfs" "binstatdfs" "binstatblock")
for val in ${layoutArray[@]}; do
        PACK_FILE="packedmodel${val}"
        META_FILE="metadata${val}.txt"
        sudo echo 3 > sudo /proc/sys/vm/drop_caches
        sudo echo 2 > sudo /proc/sys/vm/drop_caches
        sudo echo 1 > sudo /proc/sys/vm/drop_caches
	./exe  --mode inference --format binary  --metadatafilename "/data5/${META_FILE}" --labelcol 10 --layout ${layoutArray[${ARR_COUNT}]} --intertwine 4 --modelfilename "/data5/$PACK_FILE" --datafilename $DATA_FILE --logdir '/data5/logs/' --numthreads 1 --package sklearn --algorithm randomforest --task classification
        ARR_COUNT=$((ARR_COUNT+1))
done


