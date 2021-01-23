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
import pickle, csv, time, io
from sklearn.tree import *
from sklearn import tree
from csv import *
import csv
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
    #print (len(model.estimators_))
    #print (len(model.estimators_[0]))
    for estimators in model.estimators_:
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
   
    f.close()
    return X_train, X_test


def parseCmdArgs():
    parser = argparse.ArgumentParser()

    parser.add_argument('--labelcol', action='store', dest='label_column',
                    help='Label column', type=int)

    parser.add_argument('--datadir', action='store', dest='file_dir',
                    help='Dataset location directory')

    parser.add_argument('--datafilename', action='store', dest='data_filename',
                    help='Dataset name')

    parser.add_argument('--numtrees', action='store', dest='num_trees',
                    help='Number of trees')

    parser.add_argument('--numtest', action='store', dest='num_test', nargs='?',
                    const=100, type=int, help='Number of test samples')

    parser.add_argument('--algorithm', action='store', dest='algorithm',
                    help='ALgorithm (rf/gbt)')
    
    parser.add_argument('--task', action='store', dest='task',
                    help='Task (classification/regression)')

    results = parser.parse_args()

    return results



results = parseCmdArgs()

label_column = int(results.label_column)
file_dir = results.file_dir
num_trees = int(results.num_trees)
data_filename = results.data_filename
num_test = int(results.num_test)
algorithm = results.algorithm
task = results.task

data_string = data_filename.split('.')[0]
data_path_filename = os.path.join(file_dir, data_filename)
save_train_data = os.path.join(file_dir ,'train_' + data_filename)
save_test_data = os.path.join(file_dir, 'test_' + data_filename)
rf_model_filename = os.path.join(file_dir, 'rf_'+ str(num_trees) + data_string + '.json')

X, y  = load_csv(data_path_filename, label_column)
print('csv loaded')
X_train, X_test, y_train, y_test = train_test_split(
            X, y, test_size=num_test, random_state=54)
#concat_arr_train = np.c_[X_train, y_train]
concat_arr_test = np.c_[X_test, y_test]
print('concat done')
#np.savetxt(save_train_data, concat_arr_train, delimiter=",", fmt='%1.3f')
np.savetxt(save_test_data, concat_arr_test, delimiter=",", fmt='%1.3f')

if algorithm == 'rf':
    print('entered rf')
    if task == 'classification':
        model1 = RandomForestClassifier(n_estimators = num_trees, n_jobs=-1, verbose=2)
    else:
        model1 = RandomForestRegressor(n_estimators = num_trees, n_jobs=-1, verbose=2)
else:
    print('entered gb')
    if task == 'classification':
        model1 = GradientBoostingClassifier(n_estimators = num_trees, max_depth=12, random_state=1, max_features='log2', verbose=2)
    else:
        model1 = GradientBoostedRegressor(n_estimators = num_trees, random_state=1, max_features='log2', max_depth=12, verbose=2)

model1.fit(X_train,  y_train)
#dump(model1, save_filename)
if task == 'classification':
    if algorithm == 'rf':
        write_to_json(model1, rf_model_filename)
    else:
        write_to_json_gbt(model1, rf_model_filename)
else:
    if algorithm == 'rf':
        write_to_json(model1, rf_model_filename, regression=True)
    else:
        write_to_json_gbt(model1, rf_model_filename, regression=True)

