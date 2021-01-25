import numpy as np
from pprint import pprint
from sklearn.model_selection import train_test_split
from sklearn.datasets import load_iris
from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import *
from mmappickle import mmapdict
import pickle, csv, time, io
from sklearn.tree import *
from sklearn import tree
from csv import *
import csv

#GLOBAL VARIABLES
#data_filename = 'cifar-100.csv'
data_filename = '/root/pacset/data/cifar-10.csv'

random_file_size = 900000
num_files = 40

#train = False
train = True


def trainAndSave(filename, X_train, y_train):
    """
    Train a decision tree classifier model and save to a mmapped file 
    :param filename: the filename to store the model
    :param X_train: The dataset to be used to train the model
    :param y_train: Labels of dataset to be used to train the model
    :return:void
    """
    #Fit the decision tree
    #clf = DecisionTreeClassifier(random_state=1)
    clf = RandomForestClassifier(random_state=1, n_estimators=100)
    clf.fit(X_train, y_train)

    #get attribute dict that is pickled
    attr_dict = clf.__getstate__()
   
    mmapped_dict_vector = []
    #open mmapped file
    for i in range(num_files):
        mmapped_dict = mmapdict(filename + str(i) + ".pkl")
    
        #save it to mmapped file
        for key in attr_dict:
            mmapped_dict[key] = attr_dict[key]

        mmapped_dict_vector.append(mmapped_dict)

def loadModel(filename):
    dict_vec = []
    #Open the mmapped dict file
    for i in range(num_files):
        loaded_dict = mmapdict(filename + str(i) + '.pkl')
        dict_vec.append(loaded_dict)
    #tree = loaded_dict["tree_"].children_left
    #childs = tree.children_left
    return dict_vec

def writeRandomFile(filename,size):
    """
    generate big binary file with the specified size in bytes
    :param filename: the filename
    :param size: the size in bytes
    :return:void
    """
    import os 
    with open('%s'%filename, 'wb') as fout:
        fout.write(os.urandom(size)) #1


def readRandomFile(filename):
    """
    Read from large binary file
    :param filename: the filename
    :return:void
    """
    with open(filename, 'rb') as fin:
        fileContent = fin.read()


def load_iris_ext():
    """
    Loads the iris dataset
    """
    iris = load_iris()
    X = iris.data
    y = iris.target
    X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=0)
    return X_train, X_test, y_train, y_test


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
    return X_train, X_train, X_test, X_test


def load_cifar1(version):
    """
    Loads the cifar dataset
    """
    def unpickle(file):
        with io.open(file, 'r' )as fo:
            dict = pickle.load(fo, encoding ='utf-8')
        return dict

    if version == 10:
        data = unpickle('cifar-10-python/train')
    elif version == 100:
        data = unpickle('cifar-100-python/train')
    else:
        printf("Error, this does not exist")
        return

    X_train = np.array(data['data'])
    X_test = np.array(data['fine_labels'])
    return X_train, X_test, X_train, X_test



#Write random bytes to a large file 
writeRandomFile(random_filename, random_file_size)

#Load the data and split into train and test
#X_train, X_test, y_train, y_test = load_iris_ext()
X_train, X_test, y_train, y_test = load_csv(data_filename)
X_train = np.array(X_train)
X_train = X_train.astype(np.float32)
if train:
    trainAndSave(model_filename, X_train, y_train)
else:
    X_test = np.array(X_test)
    X_test = X_test.astype(np.float32)
    loaded_dict_vec = loadModel(model_filename)
    start_time = time.time()
    count = 1 
    for row in X_test:
        row_two_dims = np.expand_dims(row, axis=0)
        #read from random file to clear the cache
        readRandomFile(random_filename) 
        start_time = time.time()
        for k in loaded_dict_vec[count%num_files]["estimators_"]:
            #Predict per sample
            prediction = k.apply(row_two_dims)
        if count < 10:
            print(time.time() - start_time)
        count+=1
