# Declare an array of string with type
#declare -a layoutArray=("bfscorrect0" "dfscorrect0" "statdfscorrect0" "bfscorrect2" "dfscorrect2" )
#declare -a layoutArray2=("bfs" "dfs" "statdfs" "binbfs" "bindfs")

declare -a layoutArray=("block2048correct0" "block2048correct2" "stat2048correct2" )
declare -a layoutArray2=("blockstat" "binblockstat" "binstatdfs" )

ORIG_PACK_DIR="/data4"
NUM_FILES=10
ARR_COUNT=0
cd scripts
python3 delete_files_new.py $NUM_FILES
cd ..
for val in ${layoutArray[@]}; do
	PACK_FILE="packedmodel${val}.bin"
	META_FILE="metadata${val}.txt"
	cd scripts
	python3 copy_files_new.py $NUM_FILES $ORIG_PACK_DIR $PACK_FILE $META_FILE
	cd ..
	sudo echo 3 > sudo /proc/sys/vm/drop_caches
	sudo echo 2 > sudo /proc/sys/vm/drop_caches
	sudo echo 1 > sudo /proc/sys/vm/drop_caches
	./exe  --mode inference --format binary  --metadatafilename "/dat0/${META_FILE}" --layout ${layoutArray2[${ARR_COUNT}]} --intertwine 2 --modelfilename $PACK_FILE --datafilename /data4/foo2.csv --numthreads 1 --package sklearn --algorithm randomforest --task classification
	ARR_COUNT=$((ARR_COUNT+1))
	cd scripts
	python3 delete_files_new.py $NUM_FILES
	cd ..
done
