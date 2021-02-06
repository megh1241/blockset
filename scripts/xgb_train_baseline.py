#!/usr/bin/python
# this is the example script to use xgboost to train
import numpy as np
import csv
import time
import argparse
import xgboost as xgb

def load_csv(filename, label_col):
    """
    Loads a csv file containin the data, parses it
    and returns numpy arrays the containing the training
    and testing data along with their labels.

    :param filename: the filename
    :return: tuple containing train, test data np arrays and labels
    """

    X_train = []
    X_test = []
    num = 0
    with open(filename,'rt') as f:
        reader = csv.reader(f, delimiter=',')
        for row in reader:
            row1 = [float(item) for item in row if item != '\0']
            last_ele = row1.pop(label_col)
            X_train.append(row1)
            X_test.append(int(last_ele))
            num+=1
            if num > 1000:
                break

    f.close()
    return X_train, X_test


def parseCmdArgs():
    parser = argparse.ArgumentParser()

    parser.add_argument('--labelcol', action='store', dest='label_column',
                    help='Label column', type=int)

    parser.add_argument('--datafilename', action='store', dest='data_filename',
                    help='Dataset name')

    parser.add_argument('--numtrees', action='store', dest='num_trees', nargs='?', const=100, type=int,
                    help='Number of trees')

    parser.add_argument('--maxdepth', action='store', dest='max_depth', nargs='?', const=6, type=int,
                    help='Maximum depth of the trees')
    results = parser.parse_args()

    return results


results = parseCmdArgs()
label_col = int(results.label_column)
data_path_filename = results.data_filename
num_round = int(results.num_trees)
max_depth = int(results.max_depth)

data, label = load_csv(data_path_filename, label_col)
data = np.array(data)
label = np.array(label)
xgmat = xgb.DMatrix( data, label=label )

# setup parameters for xgboost
param = {}
# use logistic regression loss, use raw prediction before logistic transformation
# since we only need the rank
param['objective'] = 'binary:logitraw'
# scale weight of positive examples
#param['scale_pos_weight'] = sum_wneg/sum_wpos
param['eta'] = 0.1
param['max_depth'] = max_depth
param['eval_metric'] = 'auc'
param['nthread'] = 16
param['verbosity'] = 2
# you can directly throw param in, though we want to watch multiple metrics here
plst = list(param.items())+[('eval_metric', 'ams@0.15')]

watchlist = [ (xgmat,'train') ]
# boost 120 trees

print ('loading data end, start to boost trees')
bst = xgb.train( plst, xgmat, num_round, watchlist );
# save out model
bst.save_model('gbt.model')

print ('finish training')
