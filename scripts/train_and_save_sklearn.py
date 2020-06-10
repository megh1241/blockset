from sklearn.ensemble import RandomForestClassifier
from sklearn.ensemble import RandomForestRegressor
from sklearn.datasets import load_iris
from sklearn.tree import DecisionTreeClassifier
import pickle
from sklearn import datasets
import sklearn_json as skljson
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
import json
from sklearn.datasets import fetch_openml
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.utils import check_random_state
import codecs

######## GLOBALS #########
#n_trees = 2048
n_trees = 128
#data_filename = '../data/cifar-10.csv'
#data_filename = '../data/iris.csv'
data_filename = '/data5/foo2.csv'
json_filename = "/data5/foo.json"

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


def write_to_json(model1, filename):
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

    new_dict['n_estimators'] = final_count+1 
    new_dict['n_classes'] = model1.n_classes_
    json_obj = json.dumps(new_dict)
    print('finish dumping')
    with open(filename, "w") as outfile: 
        outfile.write(json_obj) 

    end_time = time.time()
    print('time taken for manual json conversion: ', end='')
    print(end_time - start_time)


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
    with open(filename,'rt') as f:
        reader = csv.reader(f, delimiter=',')
        for row in reader:
            #row_new = [i.encode('utf-8').strip() for i in row]
            row1 = [int(item) for item in row if item != '\0']
            row_int = list(np.nan_to_num(row1))
            last_ele = row_int.pop(0)
            #if num % 2:
            X_train.append(row_int)
            X_test.append(int(last_ele))
            num+=1
            print(num)
            if num > 99999995:
                break
   
    f.close()
    return X_train, X_test


###### BEGIN SCRIPT ######

#Load data
'''
iris = load_iris()
X = iris.data
y = iris.target
# Load data from https://www.openml.org/d/554
X, y = fetch_openml('mnist_784', version=1, return_X_y=True)
train_samples = 60000
t0 = time.time()
random_state = check_random_state(0)
permutation = random_state.permutation(X.shape[0])
X = X[permutation]
y = y[permutation]
X = X.reshape((X.shape[0], -1))
train_size = X.shape[0]
#scaler = StandardScaler()
#X = scaler.fit_transform(X)
'''

#Load csv
X, y  = load_csv(data_filename)
print('csv loaded')
#Load sklearn dataset
#X, y = datasets.load_diabetes(return_X_y=True)
#Train model
model1 = RandomForestClassifier(n_estimators = n_trees, n_jobs=-1)
print('classifier created')
model1.fit(X,  y)
print('model fit')

write_to_json(model1, json_filename)
print('finish write to file')
#Save dataset to csv
#save_dataset_csv(X, y, '../data/mnist.csv')

#Save model to json
#skljson.to_json(model1, '../models/mnist_new3.json')

