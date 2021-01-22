
DATA_FILE="/data5/HIGGS.csv"

declare -a layoutArray=("bfs")
#declare -a layoutArray=("binbfs" "bindfs" "binstatdfs" "binstatblock")
for val in ${layoutArray[@]}; do
        PACK_FILE="/data5/packedmodel${val}"
        META_FILE="/data5/metadata${val}.txt"
        sudo echo 3 > sudo /proc/sys/vm/drop_caches
        sudo echo 2 > sudo /proc/sys/vm/drop_caches
        sudo echo 1 > sudo /proc/sys/vm/drop_caches
	./exe  --mode inference --format binary  --metadatafilename "${META_FILE}" --labelcol 0 --layout ${layoutArray[${ARR_COUNT}]} --intertwine 1 --modelfilename "$PACK_FILE" --datafilename $DATA_FILE --logdir '/data5/logs/' --numthreads 1 --package sklearn --algorithm gradientboost --task classification
        ARR_COUNT=$((ARR_COUNT+1))
done


