#!/usr/bin/python
# this is the example script to use xgboost to train
import numpy as np
import time
import xgboost as xgb
import csv
import argparse

def load_csv(filename, label_col, num_test):
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
            #if num > num_test:
            #    break

    f.close()
    return X_train, X_test


def parseCmdArgs():
    parser = argparse.ArgumentParser()

    parser.add_argument('--labelcol', action='store', dest='label_column',
                    help='Label column', type=int)

    parser.add_argument('--datafilename', action='store', dest='data_filename',
                    help='Dataset name')

    parser.add_argument('--modelfilename', action='store', dest='model_filename',
                    help='Dataset name')

    parser.add_argument('--numtest', action='store', dest='num_test', nargs='?',
                    const=100, type=int, help='Number of test samples')

    results = parser.parse_args()

    return results


results = parseCmdArgs()
label_col = int(results.label_column)
data_path_filename = results.data_filename
model_path = results.model_filename
num_test = int(results.num_test)

# load in training data, directly use numpy
data, label = load_csv(data_path_filename, label_col, num_test)

print 'finish loading from csv '

xgmat = xgb.DMatrix( data[num_test] )
bst = xgb.Booster({'nthread': 1})  # init model
time_start = time.time()
bst.load_model(model_path)  # load data
time_end = time.time()

ypred = bst.predict(xgmat)
print "Elapsed time: "
print time_end - time_start
