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

######## GLOBALS #########
n_trees = 128
data_filename = '/root/pacset/data/cifar-10.csv'
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
            row_int.pop(0)
            X_train.append(row_int)
            X_test.append(int(row[0]))
            num+=1
            if num > 10000:
                break
    return X_train, X_test, X_train, X_test


###### BEGIN SCRIPT ######

#Load data
'''
iris = load_iris()
X = iris.data
y = iris.target
'''
X, y, a, b = load_csv(data_filename)
#Train model
model = RandomForestClassifier(n_estimators = n_trees)
#model = DecisionTreeClassifier()

model.fit(X, y)

#Save model to pickle
pickle.dump(model, open('../models/rf128.pkl', 'wb'))

#Save model to json
skljson.to_json(model, '../models/rf128.json')


