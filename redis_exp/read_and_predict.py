#### GLOBAL IMPORTS ####

import sklearn 
import numpy as np
import redis


#### GLOBAL VARIABLES ####

model_filepath = "packed_model.txt"
obs_filepath = "iris.csv"
metadata_filepath = "metadata.txt"
block_size = 128


def loadObsFromFile(filepath):
    """
    Loads a csv file containin the data, parses it
    and returns numpy arrays the containing the training
    and testing data along with their labels.

    :param filename: the filename
    :return: tuple containing train, test data np arrays and labels
    """
    X_train = []
    y_train = []
    num = 0
    with open(filename,'rt')as f:
        reader = csv.reader(f, delimiter=',')
        for row in reader:
            row_int = [int(item) for item in row]
            last_ele = row_int.pop()
            X_train.append(row_int)
            y_train.append(int(last_ele))
            num+=1
    return X_train, y_train


def readModelFromFile(filepath):
    """
    Read a packed model from file
    and store in a list of node lists
    The format of the model is:
    left_child, right_child, threshold, feature\n
    :param Filename
    :return: returns forest which is a list of nodes
    """
    with open(filepath) as f:
        forest = [[float(num) for num in line.split(",")] 
                for line in f]
    return forest


def createForestRedisDB(forest):
    """
    Create a redis DB to store nodes of
    the forest in the form of hashes where
    the keys are the node attributes
    :param forest
    :return returns a handle to the redis client
    """
    #initialize fields array
    fields = ['left', 'right', 'feature', 'threshold']

    #Create Redis client
    redisClient = redis.StrictRedis(host='localhost',
                                        port=6379,
                                        db=0)

    #Store the nodes in a redis hash table
    #Key = nodeid / blocksize
    #Fields = left, right, threshold, feature
    for counter, node in enumerate(forest):
        key = int(counter / block_size)
        for field, value in zip(fields, node): 
            redisClient.hset(key, field, value)

    return redisClient


def createObservationRedisDB(X_test):
    """
    Create a redis DB to store test data
    :param X_test
    :return returns a handle to the redis client
    """
    #Create Redis client
    redisClient = redis.StrictRedis(host='localhost',
                                        port=6379,
                                        db=0)
    #key = observation number 
    #value = redis list with feature values
    for row, index in enumerate(X_test):
        redisClient.rpush(index, *row)
    
    return redisClient


#### BEGIN SCRIPT ####

#Read list from file
forest = readModelFromFile(model_filepath)

#create the redis DB to store the nodes in the forest
redisClientForest = createForestRedisDB(forest)

#Read observation data from file
X, y = loadObsFromFile(obs_filepath)

#Create the redis DB
redisClientObservation = createObservationRedisDB(obs_data)

print(redisClientForest.hget(2, 'right'))

