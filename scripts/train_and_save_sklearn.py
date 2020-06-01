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

from sklearn.datasets import fetch_openml
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.utils import check_random_state

######## GLOBALS #########
n_trees = 128
#n_trees = 128
#data_filename = '../data/cifar-10.csv'
data_filename = '../data/iris.csv'


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
            row_int = [float(item) for item in row]
            last_ele = row_int.pop(-1)
            X_train.append(row_int)
            X_test.append(int(last_ele))
            num+=1
            print (int(last_ele))
    return X_train, X_test, X_train, X_test


###### BEGIN SCRIPT ######

#Load data
'''
iris = load_iris()
X = iris.data
y = iris.target
'''

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

#Load csv
#X, y, a, b = load_csv(data_filename)

#Load sklearn dataset
#X, y = datasets.load_diabetes(return_X_y=True)

#Train model
model1 = RandomForestClassifier(n_estimators = n_trees)
#model1 = RandomForestRegressor(n_estimators = n_trees)
model1.fit(X,  y)

#Save dataset to csv
save_dataset_csv(X, y, '../data/mnist.csv')

#Save model to pickle
pickle.dump(model1, open('../models/mnist.pkl', 'wb'))

#Save model to json
#skljson.to_json(model1, '../models/reg.json')
skljson.to_json(model1, '../models/mnist.json')

