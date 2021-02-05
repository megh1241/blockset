#List of layouts benchmarked. The best layout is binstatblock
declare -a layoutArray=("bfs" "dfs" "statdfs" "dfs" "binbfs" "bindfs" "binstatdfs" "binstatblock")

#TODO: Replace with absolute path of the model stored from the train_sklearn.sh script
MODEL_FILE="/data4/rf_682cifar-10.json"

#TODO: Replace with absolute path of the data
DATA_FILE="/data4/cifar-10.csv"

#TODO: Change to "text" for PACSET-as-a-service
FORMAT="binary"

#TODO: Change to "gradientboost" for gradient boosted trees
ALGORITHM="randomforest"

#TODO: Change to "regression" for regression 
TASK="classification"

#TODO: Change to the bin depth , i.e number of levels intertwined. It is a tuneable hyperparameter. 
INTERTWINE=4

for val in ${layoutArray[@]}; do
	#TODO: Replace with the absolute path of the packed model file where you would like to save the model 
	PACK_FILE="/data4/packedmodel${val}.bin"
	#TODO: Replace with the absolute path of the packed model file where you would like to save the metadata 
	META_FILE="/data4/metadata${val}.txt"
	./exe  --mode pack --format $FORMAT  --metadatafilename $META_FILE --layout ${val} --intertwine $INTERTWINE --packfilename $PACK_FILE --modelfilename $MODEL_FILE --datafilename $DATA_FILE --numthreads 1 --package sklearn --algorithm $ALGORITHM --task $TASK 
	done
