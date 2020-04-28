#### GLOBAL IMPORTS ####

import sklearn 
import numpy as np
import redis
import csv

#### GLOBAL VARIABLES ####

model_filepath = "packed_model.txt"
obs_filepath = "../data/iris.csv"
metadata_filepath = "../metadata.txt"
aws_dns_endpoint = "packet-001.zzfeoi.0001.use1.cache.amazonaws.com" 
block_size = 128

#TODO: Do not hardcode, read from file
num_trees = 10
num_bins = 1
num_classes = 4

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
    combined = []
    with open(filepath,'rt')as f:
        reader = csv.reader(f, delimiter=',')
        for row in reader:
            row_int = [float(item) for item in row]
            last_ele = row_int.pop()
            X_train.append(row_int)
            y_train.append(int(last_ele))
            combined.append(row_int)

    return X_train, y_train, combined


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
    redisClient = redis.StrictRedis(host=aws_dns_endpoint,
                                        port=6379,
                                        db=0)

    print('done1')
    #Store the nodes in a redis hash table
    #Key = nodeid / blocksize
    #Fields = left, right, threshold, feature
    for counter, node in enumerate(forest):
        for field, value in zip(fields, node): 
            redisClient.hset(counter, field, value)
            print('done2')
    return redisClient


def createObservationRedisDB(X_test):
    """
    Create a redis DB to store test data
    :param X_test
    :return returns a handle to the redis client
    """
    
    #Create Redis client
    redisClient = redis.StrictRedis(host=aws_dns_endpoint,
                                        port=6379,
                                        db=1)
    #key = observation number 
    #value = redis list with feature values
    for index, row in enumerate(X_test):
        redisClient.rpush(index, *row)
    
    return redisClient


def getNodeFromDB(client, node_id):
    left = int(float(client.hget(node_id, 'left')))
    right = int(float(client.hget(node_id, 'right')))
    feature = int(float(client.hget(node_id, 'feature'))) 
    threshold = float(client.hget(node_id, 'threshold'))
    return left, right, feature, threshold


def predict(client, X):
    """
    Predicts label for a test observation
    one at a time
    :param redis client and X
    :returns predicted label
    """
    
    preds = [0] * num_classes
    curr_node = [i for i in range(num_trees)]

    while True:
        number_not_in_leaf = 0
        for i in range(num_trees):
            if curr_node[i] >= 0:
                left, right, feature, threshold =\
                        getNodeFromDB(client, curr_node[i])
                if left > -1:
                    obs_feature_val = X[feature]
                    if obs_feature_val <= threshold:
                        curr_node[i] = left
                    else:
                        curr_node[i] = right
                    number_not_in_leaf += 1
        
        if number_not_in_leaf <= 0:
            break

    for i in range(num_trees):
        class_label = int(float(client.hget(curr_node[i], 'right')))
        preds[class_label] += 1 
    
    return np.argmax(preds)


def validatePredictions(predicted, y):
    right = 0
    total = 0
    for i, j in zip(predicted, y):
        if i==j:
            right+=1
        total+=1

    return float(right) / float(total)



###############################
######## BEGIN SCRIPT #########
###############################

#Read list from file
forest = readModelFromFile(model_filepath)

#create the redis DB to store the nodes in the forest
redisClientForest = createForestRedisDB(forest)

#Read observation data from file
X, y, combined = loadObsFromFile(obs_filepath)

#Create the redis DB
redisClientObservation = createObservationRedisDB(X)

#Predict
num_observations = len(X)
num_features = len(X[0])
predicted = []
for j in range(num_observations):
    row_db = redisClientObservation.lrange(j, 0, num_features)
    row = [float(i) for i in row_db]
    predict_value = predict(redisClientForest, row)
    predicted.append(predict_value)

#Check if predicted values match actual labels
accuracy = validatePredictions(predicted, y)
print("Accuracy: ")
print(accuracy)

#Flush the DBs
#redisClientForest.flushdb()
#redisClientObservation.flushdb()
