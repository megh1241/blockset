#List of layouts benchmarked. The best layout is binstatblock
declare -a layoutArray=("binstatdfs")

#TODO: Replace with absolute path of the model stored from the train_sklearn.sh script
MODEL_FILE="/data/rf100HIGGS.json"

#TODO: Replace with absolute path of the data. #TODO: Replace with label column (-1 indicates last column, 0 would indicate first column). Note the data should be in a csv file with columns representing features (and one of the columns holding the labels), and rows representing observations.
DATA_FILE="/data/HIGGS.csv"

#TODO: Change to "text" for BLOCKSET-as-a-service
FORMAT="binary"

#TODO: Change to "gradientboost" for gradient boosted trees
ALGORITHM="randomforest"

#TODO: Change to "regression" for regression 
TASK="classification"


#TODO: Change to the bin depth , i.e number of levels intertwined. It is a tuneable hyperparameter. 
INTERTWINE=4

#TODO: Change to the blocksize. For the BLOCKSET-as-a-service experiments, the blocksize should be 8 or 16.
BLOCKSIZE=16

for val in ${layoutArray[@]}; do
	#TODO: Replace with the absolute path of the packed model file where you would like to save the model 
	PACK_FILE="/data/packedmodel${val}.bin"
	#TODO: Replace with the absolute path of the packed model file where you would like to save the metadata 
	META_FILE="/data/metadata${val}.txt"
	./exe  --labelcol '0' --batchsize 1 --mode pack --format $FORMAT --blocksize $BLOCKSIZE --metadatafilename $META_FILE --layout ${val} --logdir "logs/" --numbins 8 --intertwine $INTERTWINE --packfilename $PACK_FILE --modelfilename $MODEL_FILE --datafilename $DATA_FILE --numthreads 1 --package sklearn --algorithm $ALGORITHM --task $TASK 
	done
