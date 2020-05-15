from sklearn.ensemble import RandomForestClassifier
from sklearn.datasets import load_iris
from sklearn.tree import DecisionTreeClassifier
import pickle
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
data_filename = '../data/cifar-10.csv'
#data_filename = '../data/iris.csv'
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
            row_int = [int(item) for item in row]
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
'''
X, y = fetch_openml('mnist_784', version=1, return_X_y=True)
train_samples = 60000
t0 = time.time()
random_state = check_random_state(0)
permutation = random_state.permutation(X.shape[0])
X = X[permutation]
y = y[permutation]
X = X.reshape((X.shape[0], -1))
train_size = X.shape[0]
X_train, X_test, y_train, y_test = train_test_split(
    X, y, train_size=train_samples)
'''
#scaler = StandardScaler()
#X_train = scaler.fit_transform(X_train)
#X_test = scaler.transform(X_test)
#X = X_train

X, y, a, b = load_csv(data_filename)
#Train model
model1 = RandomForestClassifier(n_estimators = n_trees)
model1.fit(X,  y)
#model = Decis_set_oob_scoreionTreeClassifier()

#model._loadX, y)

#model = pickle.load('../models/rf128.pkl')
#print(model1.score(X, y))

#Save model to pickle
#pickle.dump(model, open('../models/rf128.pkl', 'wb'))

#Save model to json
skljson.to_json(model1, '../models/cifar128.json')
#skljson.to_json(model1, '../models/iris10.json')
