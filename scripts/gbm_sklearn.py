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
data_filename = 'cifar-10.csv'
json_filename = 'xgb_model.json'
num_files = 40
num_trees = 10
BLOCK_SIZE = 128
depth_intertwined = 2



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
    clf = GradientBoostingClassifier(max_depth = 6, random_state=1, n_estimators=num_trees)
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
            row_int = [int(item) for item in row]
            row_int.pop()
            X_train.append(row_int)
            X_test.append(int(row[-1]))
            num+=1
    return X_train,  X_test, X_train, X_test


# To apply a classifier on this data, we need to flatten the image, to
# turn the data in a (samples, feature) matrix:
#n_samples = len(digits.images)
#data = digits.images.reshape((n_samples, -1))

# Split data into train and test subsets
#X_train, X_test, y_train, y_test = train_test_split(
#    data, digits.target, test_size=0.95, shuffle=False)

#X_train, y_train, X_test, y_test = load_csv(data_filename)
X_train, y_train = load_iris(return_X_y=True)
#X_train = np.array(X_train)
#X_train = X_train.astype(np.float32)
gb_clf = trainAndSave(X_train, y_train)
write_to_json(gb_clf, json_filename)
'''
trees = []
for i in range(len(rf_clf.estimators_)):
    for j in range(3):
        trees.append(rf_clf.estimators_[i,j])
'''
