#### GLOBAL IMPORTS ###

import sklearn 
import numpy as np
import redis
import csv
import time

#### GLOBAL VARIABLES ####

model_filepath = "../models/packedbinwdfsstat16.txt"
obs_filepath = "../data/cifar-10.csv"
metadata_filepath = "../models/metabinwdfsstat16.txt"
aws_dns_endpoint = "lambda-redis.zzfeoi.ng.0001.use1.cache.amazonaws.com"
blocksize = 16

#TODO: Do not hardcode, read from file
bin_sizes = []
bin_node_sizes = []
tree_start = []

cached_node_hash = []

def loadModelMetadataFromFile(metadata_filepath):
    """
    Loads model metadata from file
    sets global metadata
    vars to the appropriate val
    """
    readlines = []

    with open(metadata_filepath, 'rt') as f:
        for line in f:
            readlines.append(line)
    
    metadata_int = [int(float(i)) for i in readlines]

    num_classes = metadata_int[0]
    num_bins = metadata_int[1]

    k = 2
    for i in range(num_bins):
        bin_sizes.append(metadata_int[k]) 
        k+=1

    for i in range(num_bins):
        bin_node_sizes.append(metadata_int[k])
        k+=1

    for i in range(num_bins):
        temp = []
        for j in range(bin_sizes[i]):
            temp.append(metadata_int[k])
            k+=1
        tree_start.append(temp)

    
    return num_bins, num_classes, bin_node_sizes


def loadObsFromFile(filepath):
    """
    Loads a csv file containin the data, parses it
    and returns numpy arrays the containing the training
    and testing data along with their labels.

    :param filename: the filename
    :return: tuple containing train, test data np arrays and labels
    """
    i = 1   
    X_train = []
    y_train = []
    combined = []
    with open(filepath,'rt') as f:
        reader = csv.reader(f, delimiter=',')
        for row in reader:
            row_int = [int(float(item)) for item in row]
            last_ele = row_int.pop(-1)
            X_train.append(row_int)
            y_train.append(int(float(last_ele)))
            '''
            if i >= 10000:
                break
            i+=1
            '''
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
    redisClient = redis.StrictRedis(host=aws_dns_endpoint,
                                        port=6379,
                                        db=0)

    redisClient.flushdb()
    #Store the nodes in a redis hash table
    for counter, node in enumerate(forest):
        block_num = int(counter / blocksize)
        for field, value in zip(fields, node): 
            redisClient.hset(counter, field, value)
    return redisClient


def createForestBlockNodesRedisDB(forest, bin_node_sizes, num_bins, num_classes):
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

    redisClient.flushdb()
    bin_num_curr = 0
    count = 0
    pos = 0
    #Store the nodes in a redis hash table
    for node in forest:
        block_num = int(count / blocksize)
        redisClient.rpush(str(bin_num_curr) + ':' + str(block_num), count, 
                node[0], node[1], node[2], node[3])
        count += 1
        if count == bin_node_sizes[bin_num_curr]:
            count = 0
            bin_num_curr += 1

    return redisClient


def createObservationRedisDB(X_test, num_bins, num_classes):
    """
    Create a redis DB to store test data
    :param X_test
    :return returns a handle to the redis client
    """
    
    #Create Redis client
    redisClient = redis.StrictRedis(host=aws_dns_endpoint,
                                        port=6379,
                                        db=1)
    redisClient.flushdb()
    #key = observation number 
    #value = redis list with feature values
    for index, row in enumerate(X_test):
        redisClient.rpush(index, *row)
    return redisClient


def createLabelRedisDB(y):
    #Create Redis client
    redisClient = redis.StrictRedis(host=aws_dns_endpoint,
                                        port=6379,
                                        db=2)
    redisClient.flushdb()
    for index, value in enumerate(y):
        redisClient.set(index, value)
    return redisClient


def createMetaRedisDB(X):
    #Create Redis client
    redisClient = redis.StrictRedis(host=aws_dns_endpoint,
                                        port=6379,
                                        db=3)

    redisClient.flushdb()

    redisClient.set("num_features", len(X[0]))
    redisClient.set("num_observations", len(X))
    redisClient.set("num_classes", num_classes)
    #redisClient.set("num_trees", num_trees)
    redisClient.set("num_bins", num_bins)
    redisClient.set("block_size", blocksize)

    binnum = 0
    for curr_bin in tree_start:
        for start_pos in curr_bin:
            redisClient.rpush("tree_start:"+ str(binnum), start_pos)
        binnum += 1
   
    for node in bin_sizes:
        redisClient.rpush("bin_sizes", node)

    for node in bin_node_sizes:
        redisClient.rpush("bin_node_sizes", node)

    return redisClient


def getNodeFromDB(client, node_id):
    left = int(float(client.hget(node_id, 'left')))
    right = int(float(client.hget(node_id, 'right')))
    feature = int(float(client.hget(node_id, 'feature'))) 
    threshold = float(client.hget(node_id, 'threshold'))
    return left, right, feature, threshold


def test(client, num_bins):
    for i in range(num_bins):
        for j in range(bin_node_sizes[i]):
            key_str = str(i) + ':' + str(int(j/blocksize))
            nodes = client.lrange(key_str, 0, -1)
            if len(nodes) == 0:
                print('blocknum')
                print(int(j/blocksize))
                print('binnum')
                print(i)


def getBlockNodeFromDB(client, node_id, bin_num):
    if node_id not in cached_node_hash[bin_num]:
        block_num = int(node_id / blocksize)
        key_str = str(bin_num) + ':' + str(block_num)
        nodes = client.lrange(key_str, 0, -1)
        siz = len(nodes)
        for i in range(0, siz, 5):
            node = ( int(float(nodes[i+1])), int(float(nodes[i+2])), 
                    int(float(nodes[i+3])), float(nodes[i+4]) )
            cached_node_hash[bin_num][int(float(nodes[i]))] = node
    return cached_node_hash[bin_num][node_id] 


def predict(client, X, num_bins, num_classes):
    """
    Predicts label for a test observation
    one at a time
    :param redis client and X
    :returns predicted label
    """
   
    preds = [0] * num_classes

    for bin_no in range(num_bins):
        curr_node = [i for i in tree_start[bin_no]]

        while True:
            number_not_in_leaf = 0
            for i in range(bin_sizes[bin_no]):
                if curr_node[i] >= 0:
                    left, right, feature, threshold =\
                            getBlockNodeFromDB(client, curr_node[i], bin_no)
                    if left > -1 :
                        obs_feature_val = X[feature]
                        if obs_feature_val <= threshold:
                            curr_node[i] = left
                        else:
                            curr_node[i] = right
                        number_not_in_leaf += 1
        
            if number_not_in_leaf <= 0:
                break

        for i in range(bin_sizes[bin_no]):
            left, class_label, feature, threshold =\
                            getBlockNodeFromDB(client, curr_node[i], bin_no)
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


#load metadata from file
num_bins, num_classes, bin_node_sizes = loadModelMetadataFromFile(metadata_filepath)

print("num bins outside meta: ")
print(num_bins)
#Read list from file
forest = readModelFromFile(model_filepath)

#create the redis DB to store the nodes in the forest
redisClientForest = createForestBlockNodesRedisDB(forest, bin_node_sizes, num_bins, num_classes)

#Read observation data from file
X, y = loadObsFromFile(obs_filepath)

#Create the redis DB
redisClientObservation = createObservationRedisDB(X, num_bins, num_classes)
print('obs created')

#create the redis DB to store labels
redisClientLabel = createLabelRedisDB(y)
print('label created')

#create the redis DB to store metadata
redisClientMetadata = createMetaRedisDB(X)

print('metadata created')
#Predict
num_observations = len(X)
num_features = len(X[0])
predicted = []
redisClientObservation = redis.StrictRedis(host=aws_dns_endpoint,
                                            port=6379,
                                            db=1)
redisClientForest = redis.StrictRedis(host=aws_dns_endpoint,
                                            port=6379,
                                            db=0)
'''
#ans = test(redisClientForest, num_bins)
sum_1 = 0
avg_1 = 0
time_start_main = time.time()
for j in range(100):
    row_db = redisClientObservation.lrange(j, 0, num_features)
    row = [int(float(i)) for i in row_db]
    time_start = time.time()
    cached_node_hash = [{} for i in range(num_bins)]
    predict_value = predict(redisClientForest, row, num_bins, num_classes)
    time_end = time.time()
    print(time_end - time_start, end=",")
    sum_1 += time_end - time_start
    avg_1 = float(sum_1) / float(j+1)
    predicted.append(predict_value)
print("DONE*********")
print(time.time() - time_start_main)
#Check if predicted values match actual labels
accuracy = validatePredictions(predicted, y)
print("Accuracy: ")
print(accuracy)
'''
