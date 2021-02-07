#Example
#TODO: Replace with name of dataset
DATA_FILENAME='cifar-10.csv'

#TODO: Replace with path to directory containing the dataset. The model will be saved in this directory as well.
DATA_DIR='/data/'

#TODO: Replace with label column (-1 indicates last column, 0 would indicate first column). Note the data should be in a csv file with columns representing features (and one of the columns holding the labels), and rows representing observations.

LAB_COL=-1

#TODO: Replace with number of trees
NUM_TREES=128

#TODO: Algorithm: rf: random forest, gbt: gradient boosted tree
ALGORITHM='rf'

#TODO: Task: Classification or regression
TASK='classification'

#TODO: Change to joblib ONLY for the baseline experiments. Otherwise it should be json.
FORMAT='json'

#NOTE: RUN python3 scripts/train_and_save_sklearn.py --help for a list of all options
#Run this script to train the model and save it in a json format for BLOCKSET to pack. One can modify this script to work for XGBoost as well, i.e the model needs to be stored in a JSON format detailed in the script.
python3 scripts/train_and_save_sklearn.py --labelcol $LAB_COL --datadir $DATA_DIR --datafilename $DATA_FILENAME --numtrees $NUM_TREES --algorithm $ALGORITHM --numtest $NUM_TEST --task $TASK --saveformat $FORMAT
