
DATA_FILE="/data5/HIGGS.csv"
#DATA_FILE="/data5/test_HIGGS.csv"

declare -a layoutArray=("bfs" "dfs" "statdfs" "statblock")
#declare -a layoutArray=("binbfs" "bindfs" "binstatdfs" "binstatblock")
for val in ${layoutArray[@]}; do
        PACK_FILE="packedmodel${val}"
        META_FILE="metadata${val}.txt"
        sudo echo 3 > sudo /proc/sys/vm/drop_caches
        sudo echo 2 > sudo /proc/sys/vm/drop_caches
        sudo echo 1 > sudo /proc/sys/vm/drop_caches
	./exe  --mode inference --logdir /data5/logs --format binary  --metadatafilename "/data5/${META_FILE}" --labelcol 0 --layout ${layoutArray[${ARR_COUNT}]} --intertwine 4 --modelfilename "/data5/$PACK_FILE" --datafilename $DATA_FILE --logdir '/data5/logs/' --numthreads 1 --package sklearn --algorithm gradientboost --task classification
        ARR_COUNT=$((ARR_COUNT+1))
done


