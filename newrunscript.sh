DATA_FILENAME='HIGGS.csv'
DATA_DIR='/data8/'
LAB_COL=0
NUM_TREES=682
ALGORITHM='rf'
TASK='classification'

python3 scripts/train_and_save_sklearn.py --labelcol $LAB_COL --datadir $DATA_DIR --datafilename $DATA_FILENAME --numtrees $NUM_TREES --algorithm $ALGORITHM --numtest $NUM_TEST --task $TASK


DATA_FILENAME='HIGGS.csv'
DATA_DIR='/data8/'
LAB_COL=0
NUM_TREES=2048
ALGORITHM='gbt'
TASK='classification'

python3 scripts/train_and_save_sklearn.py --labelcol $LAB_COL --datadir $DATA_DIR --datafilename $DATA_FILENAME --numtrees $NUM_TREES --algorithm $ALGORITHM --numtest $NUM_TEST --task $TASK
