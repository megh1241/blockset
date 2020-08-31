import numpy as np
from pprint import pprint
from sklearn.model_selection import train_test_split
from sklearn.datasets import load_iris
from sklearn.tree import *
from sklearn.ensemble import *
#from mmappickle import mmapdict
import pickle, csv, time, io
from sklearn.tree import *
from sklearn import tree
import csv, json
from sklearn import datasets, svm, metrics
from sklearn.model_selection import train_test_split
from joblib import dump, load

#GLOBAL VARIABLES
#data_filename = 'cifar-100.csv'
#data_filename = '/data9/cifar-10.csv'
data_filename = '/data9/HIGGS.csv'
#json_filename = '/data9/xgb_model_cifar.json'
json_filename = '/data9/xgb_HIGGS.json'
num_trees = 2048



def argmax_1(a):
        return max(range(len(a)), key=lambda x: a[x])


def write_to_json(model, filename, regression=False):
    start_time = time.time()
    final_count = 0
    new_dict = {'estimators': {'nodes': [], 'values': [] } }
    final_count = 0
    print (len(model.estimators_))
    print (len(model.estimators_[0]))
    for estimators in model.estimators_:
        for count, estimator in enumerate(estimators):
            nodes = estimator.tree_.__getstate__()['nodes'].tolist()
            newnodes = [list((i[0], i[1], i[2], i[3], i[5])) for i in nodes]
            length = len(nodes)
            values = estimator.tree_.__getstate__()['values']
            for i in range(length):
                if newnodes[i][0] == -1:
                    print(values[i][0])
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


def trainAndSave(X_train, y_train):
    """
    Train a decision tree classifier model and save to a mmapped file 
    :param filename: the filename to store the model
    :param X_train: The dataset to be used to train the model
    :param y_train: Labels of dataset to be used to train the model
    :return:void
    """
    #Fit the decision tree
    clf = GradientBoostingClassifier(max_depth = 10, max_features='log2', random_state=1, n_estimators=num_trees)
    clf.fit(X_train, y_train)
    return clf


def load_csv(filename):
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
    with open(filename,'rt')as f:
        reader = csv.reader(f, delimiter=',')
        for row in reader:
            row_float = [float(item) for item in row]
            X_test.append(int(row_float[0]))
            row_float.pop(0)
            X_train.append(row_float)
            num+=1
    return X_train, X_test, X_train, X_test


import numpy as np
from sklearn.ensemble import GradientBoostingClassifier as GBC
import math
import pandas as pd
''' 
# Load training data
data_train = np.loadtxt( '/data9/HIGGS.csv', delimiter=',', skiprows=1, converters={32: lambda x:int(x=='s'.encode('utf-8')) } )
 
# Pick a random seed for reproducible results. Choose wisely!
np.random.seed(42)
# Random number for training/validation splitting
r =np.random.rand(data_train.shape[0])
 
# Put Y(truth), X(data), W(weight), and I(index) into their own arrays
# First 90% are training
Y_train = data_train[:,32][r<0.9]
X_train = data_train[:,1:31][r<0.9]
W_train = data_train[:,31][r<0.9]
# Lirst 10% are validation
Y_valid = data_train[:,32][r>=0.9]
X_valid = data_train[:,1:31][r>=0.9]
W_valid = data_train[:,31][r>=0.9]
 '''
# Train the GradientBoostingClassifier using our good features
gbc = GBC(n_estimators=682, max_depth=12,min_samples_leaf=200,max_features=10,verbose=1)
X_train = pd.read_csv("/data9/HIGGS.csv", nrows = (11000000 - 500000), header = None)
Y_train = np.int8(X_train[0])
X_train = np.asarray(X_train.drop([0], axis = 1))
gbc.fit(X_train,Y_train) 
write_to_json(gbc, json_filename)
