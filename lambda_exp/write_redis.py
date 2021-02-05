import sklearn
import numpy as np
import redis
import csv
import time
import os
import codecs
from csv import *
import argparse



#TODO: replace wiht observation file path
obs_filepath = "/data4/HIGGS.csv"

#TODO: replace with filiepath of the directory containing the model 
model_dir="/data4/gbt_higgs_2048"

#TODO: Replace with the column containing the label
labelcol=0

#TODO: replace with your redis dns endpoint
aws_dns_endpoint = "Your-redis-dns-endpoint"

#TODO: Replace with the blocksize used to train 
blocksize = 16

cached_node_hash = []

def getNodeFromDB(client, node_id):
    left = int(float(client.hget(node_id, 'left')))
    right = int(float(client.hget(node_id, 'right')))
    feature = int(float(client.hget(node_id, 'feature')))
    threshold = float(client.hget(node_id, 'threshold'))
    return left, right, feature, threshold



def loadModelMetadataFromFile(metadata_filepath):
    """
    Loads model metadata from file
    sets global metadata
    vars to the appropriate val
    """
    bin_sizes = []
    bin_node_sizes = []
    tree_start = []
    readlines = []

    with open(metadata_filepath, 'rt') as f:
        for line in f:
            readlines.append(line)

    metadata_int = [int(float(i)) for i in readlines]

    num_classes = metadata_int[0]
    num_bins = metadata_int[1]

    print('num classes: ', end='')
    print(num_classes)
    print('num bins: ', end='')
    print(num_bins)

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

    print('bin sizes: ', end='')
    print(bin_sizes)
    print('bin node sizes: ', end='')
    print(bin_node_sizes)
    return num_bins, num_classes, bin_node_sizes, bin_sizes, tree_start

def loadObsFromFile(filepath, labelcol):
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
    with open(filepath, 'rt') as f:
        reader = csv.reader(f, delimiter=',')
        for row in reader:
            row_int = [float(item) for item in row if item != '\0']
            last_ele = row_int.pop(labelcol)
            X_train.append(row_int)
            y_train.append(int(last_ele))
            i+=1
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

def createForestBlockNodesRedisDB(forest, bin_node_sizes, num_bins, num_classes, db_id):
    """
    Create a redis DB to store nodes of
    the forest in the form of hashes where
    the keys are the node attributes
    :param forest, bin_node_sizes, num_bins, num_classes, db_id
    :return returns a handle to the redis client
    """

    #initialize fields array
    fields = ['left', 'right', 'feature', 'threshold']

    #Create Redis client
    redisClient = redis.StrictRedis(host=aws_dns_endpoint,
                                        port=6379,
                                        db=db_id)

    redisClient.flushdb()
    bin_num_curr = 0
    count = 0
    pos = 0
    #Store the nodes in a redis hash table
    print('forest size: ', end='')
    print(len(forest))
    for node in forest:
        block_num = int(count / blocksize)
        redisClient.rpush(str(bin_num_curr) + ':' + str(block_num), count,
                node[0], node[1], node[2], node[3])
        count += 1
        if count == bin_node_sizes[bin_num_curr]:
            print(count)
            count = 0
            bin_num_curr += 1

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
                                        db=0)
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
                                        db=1)
    redisClient.flushdb()
    for index, value in enumerate(y):
        redisClient.set(index, value)
    return redisClient

def createMetaRedisDB(X, num_bins, num_classes, bin_node_sizes, bin_sizes, tree_start, dbid, blk):
    #Create Redis client
    redisClient = redis.StrictRedis(host=aws_dns_endpoint,
                                        port=6379,
                                        db=dbid)

    redisClient.set("num_features", len(X[0]))
    redisClient.set("num_observations", len(X))
    redisClient.set("num_classes", num_classes)
    redisClient.set("num_bins", num_bins)
    redisClient.set("block_size", blk)

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


model_filenames = ['packedmodel' + str(i) + '.txt' for i in blocksizes]
meta_filenames = ['metadata' + str(i) + '.txt' for i in blocksizes]

#TODO: Replace the following lines to read the model and metadata files
model_filepaths = [os.path.join(model_dir, model_filename) for model_filename in model_filenames]
metadata_filepaths = [os.path.join(model_dir, meta_filename) for meta_filename in meta_filenames]

#Read observation data from file
X, y = loadObsFromFile(obs_filepath, labelcol)
#blocksize = blocksizes[it]
redisClientObservation = createObservationRedisDB(X)
redisClientLabel = createLabelRedisDB(y)
db_id = 2
iter1 = 0
for model_filepath, meta_filepath in zip(model_filepaths, metadata_filepaths):
    #load metadata from file
    #blocksize = blocksizes[iter1]
    num_bins, num_classes, bin_node_sizes, bin_sizes, tree_start  = loadModelMetadataFromFile(metadata_filepaths[iter1])
    print(bin_node_sizes)
    print('metadata loaded from file')
    #Read list from file
    forest = readModelFromFile(model_filepath)
    print('model loaded from file')
    #create the redis DB to store the nodes in the forest
    redisClientForest = createForestBlockNodesRedisDB(forest, bin_node_sizes, num_bins, num_classes, db_id)
    print('forest created')
    #create the redis DB to store metadata
    redisClientMetadata = createMetaRedisDB(X, num_bins, num_classes, bin_node_sizes, bin_sizes, tree_start, db_id, blocksize)
    print('metadata created')
    print('done: ', end=', ')
    print(model_filepath)
    db_id += 1
    iter1+=1
