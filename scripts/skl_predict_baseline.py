import csv
from sklearn.ensemble import *
from sklearn.datasets import load_iris
from sklearn.tree import DecisionTreeClassifier
import pickle
from sklearn import datasets
import numpy as np
from pprint import pprint
from sklearn.model_selection import train_test_split
from sklearn.datasets import load_iris
from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import *
import pickle, time, io
from sklearn.tree import *
from sklearn import tree
from sklearn.datasets import fetch_openml
import time
import matplotlib.pyplot as plt
import numpy as np
import json, os
import argparse
from sklearn.datasets import fetch_openml
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.utils import check_random_state
import codecs
from joblib import dump, load
######## GLOBALS #########

def save_dataset_csv(X, y, filename):
    """
    Saves training data to filename.csv

    :param filename: Name of file to save it to
    :param X: training data (features)
    :param y: training data (labels)
    :return: null
    """
    concat = np.concatenate((np.array(X), np.reshape(np.array(y).T, (-1, 1))), axis = 1)

    print('X size: ', end=' ')
    print(np.array(X).shape)

    print('y size: ', end=' ')
    print(np.reshape(np.array(y).T, (-1, 1)).shape)

    print('concat size: ', end=' ')
    print(concat.shape)

    print (concat.shape)
    np.savetxt(filename, concat, delimiter=",", fmt='%s')


def argmax_1(a):
    return max(range(len(a)), key=lambda x: a[x])


def write_to_json(model1, filename, regression=False):
    start_time = time.time()
    final_count = 0
    new_dict = {'estimators': {'nodes': [], 'values': [] } }
    for count, estimator in enumerate(model1.estimators_):
        nodes = estimator.tree_.__getstate__()['nodes'].tolist()
        newnodes = [list((i[0], i[1], i[2], i[3], i[5])) for i in nodes]
        length = len(nodes)
        values = estimator.tree_.__getstate__()['values']
        for i in range(length):
            if newnodes[i][0] == -1:
                newnodes[i][2] = argmax_1(list(values[i][0]))
    
        new_dict['estimators']['nodes'].append(newnodes)
        final_count = count

    if regression:
        new_dict['n_classes'] = -1
    else:
        new_dict['n_classes'] = model1.n_classes_

    new_dict['n_estimators'] = final_count+1 
    json_obj = json.dumps(new_dict)
    print('finish dumping')
    with open(filename, "w") as outfile: 
        outfile.write(json_obj) 

    end_time = time.time()
    print('time taken for manual json conversion: ', end='')
    print(end_time - start_time)


def write_to_json_gbt(model, filename, regression=False):
    start_time = time.time()
    final_count = 0
    new_dict = {'estimators': {'nodes': [], 'values': [] } }
    final_count = 0
    print('tot est: ')
    times_add=0
    print (len(model.estimators_))
    for estimators in model.estimators_:
        print(len(estimators))
        for count, estimator in enumerate(estimators):
            nodes = estimator.tree_.__getstate__()['nodes'].tolist()
            newnodes = [list((i[0], i[1], i[2], i[3], i[5])) for i in nodes]
            length = len(nodes)
            values = estimator.tree_.__getstate__()['values']
            for i in range(length):
                if newnodes[i][0] == -1:
                    #print(values[i][0])
                    newnodes[i][3] = values[i][0][0]
                    #newnodes[i][2] = argmax_1(list(values[i][0]))
            final_count += 1
            new_dict['estimators']['nodes'].append(newnodes)
            times_add+=1

    print("total total: ")
    print(times_add)
    if regression:
        new_dict['n_classes'] = -1
    else:
        new_dict['n_classes'] = model.n_classes_

    new_dict['n_estimators'] = final_count
    json_obj = json.dumps(new_dict)
    print('finish dumping')
    with open(filename, "w") as outfile:
        outfile.write(json_obj)

    end_time = time.time()
    print('time taken for manual json conversion: ', end='')
    print(end_time - start_time)


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
            if num > num_test:
                break
   
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
label_column = int(results.label_column)
data_path_filename = results.data_filename
model_filename = results.model_filename
num_test = int(results.num_test)

X, y  = load_csv(data_path_filename, 0, num_test)
print('csv loaded')
start_time = time.time()
model1 = load(model_filename)
end_time = time.time()

model1.predict(X)
print('time: ', end_time - start_time)
