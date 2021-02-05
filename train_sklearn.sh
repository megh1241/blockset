#Example
#TODO: Replace with name of dataset
DATA_FILENAME='cifar-10.csv'

#TODO: Replace with path to directory containing the dataset. The model will be saved in this directory as well.
DATA_DIR='/data3/'

#TODO: Replace with label column (-1 indicates last column, 0 would indicate first column)
LAB_COL=-1

#TODO: Replace with number of trees
NUM_TREES=682

#TODO: Algorithm: rf: random forest, gbt: gradient boosted tree
ALGORITHM='rf'

#TODO: Task: Classification or regression
TASK='classification'

#NOTE: RUN python3 scripts/train_and_save_sklearn.py --help for a list of all options
#Run this script to train the model and save it in a json format for PACSET to pack. One can modify this script to work for XGBoost as well, i.e the model needs to be stored in a JSON format detailed in the script.
python3 scripts/train_and_save_sklearn.py --labelcol $LAB_COL --datadir $DATA_DIR --datafilename $DATA_FILENAME --numtrees $NUM_TREES --algorithm $ALGORITHM --numtest $NUM_TEST --task $TASK