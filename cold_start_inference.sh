#TODO: Replace with the full path of the dataset
DATA_FILE="/data/cifar-10.csv"

#TODO: Replace this with the absolute path of the directory in which the packed model is located. 
ORIG_PACK_DIR="/data/"

#TODO replace with the number of copies you want to make of the model. Each inference is done on a difference copy of the model in a cyclical manner to ensure cold start latency. Note that This is only for the benchmarks to ensure a cold start latency.
NUM_FILES=8

#TODO: Replace with desired location of the logs(inference latency and block sizes). Note: You have to create the log directory.
LOG_DIR='/data/logs_cifar/'

#TODO: Replace it with the column containing the label in the test data
LAB_COL='10'

#TODO: Replace it with the block size
BLOCKSIZE=128

#TODO: Replace with batchsize, i.e number of inference sample per batch (for benchmark purposes)
BATCHSIZE=1

#TODO: Change to "gradientboost" for gradient boosted trees
ALGORITHM="randomforest"

#TODO: Change to "regression" for regression
TASK="classification"

declare -a layoutArray=("bfs" "dfs" "bindfs" "binstatdfs" "binbfs" "binstatblock")
for val in ${layoutArray[@]}; do
        #TODO: You can replace this with the full absolute path of the packed model and metadata
        PACK_FILE="packedmodel${val}"
        META_FILE="metadata${val}.txt"
        python3 scripts/copy_files_single_dir.py $NUM_FILES $ORIG_PACK_DIR $PACK_FILE
        sudo echo 3 > sudo /proc/sys/vm/drop_caches
        sudo echo 2 > sudo /proc/sys/vm/drop_caches
        sudo echo 1 > sudo /proc/sys/vm/drop_caches
        ./exe  --batchsize $BATCHSIZE --blocksize $BLOCKSIZE --mode inference --logdir $LOG_DIR --format binary --metadatafilename "${ORIG_PACK_DIR}/${META_FILE}" --labelcol $LAB_COL --layout ${layoutArray[${ARR_COUNT}]} --intertwine 4 --modelfilename "${ORIG_PACK_DIR}/${PACK_FILE}" --numfiles $NUM_FILES --datafilename $DATA_FILE --numthreads 1 --package sklearn --algorithm $ALGORITHM --task $TASK
        ARR_COUNT=$((ARR_COUNT+1))
done
