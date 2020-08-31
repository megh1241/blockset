import xgboost as xgb

from sklearn.datasets import dump_svmlight_file
from sklearn.model_selection import train_test_split
from sklearn import datasets

import csv, json, re, sys, operator
import numpy as np

json_filename = '/data9/cifar2048_xgb.json'
data_filename = '/data9/cifar-10.csv'
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
            print("******************************************************************************")
            #row_new = [i.encode('utf-8').strip() for i in row]
            row1 = [int(item) for item in row if item != '\0']
            row_int = list(np.nan_to_num(row1))
            last_ele = row_int.pop(-1)
            #if num % 2:
            X_train.append(row_int)
            X_test.append(int(last_ele))
            num+=1
            print(num)

    f.close()
    return X_train, X_test


def unpackNodes(txt_model):
    internal_nodes_string = list(re.findall('([0-9]+):\[f([0-9]+)<([0-9]+.[0-9]+)\] yes=([0-9]+),no=([0-9]+)',txt_model))
    leaf_nodes_string = list(re.findall('([0-9]+):leaf=([-+]?[0-9]+.[0-9]+)', txt_model))
    end_leaf_nodes_string = list(re.findall('([0-9]+):leaf=[-+]?[0-9]+.[0-9]+,cover=[-+]?[0-9]+.[0-9]+\nbooster', txt_model))
    print(len(end_leaf_nodes_string))
    last_line = int(list(re.findall('.*$', txt_model))[0].strip('\t').split(':')[0])
    print('last line')
    print(last_line)    
    internal_nodes = []
    leaf_nodes = []
    end_leaf_nodes = []
    for node in internal_nodes_string:
        internal_nodes.append([int(node[0]), int(node[1]), float(node[2]), int(node[3]), int(node[4])])
    
    for node in leaf_nodes_string:
        leaf_nodes.append([int(node[0]), float(node[1])])

    for node in end_leaf_nodes_string:
        end_leaf_nodes.append(int(node))

    end_leaf_nodes.append(last_line)

    final_nodes = []
    temp = []
    for node in internal_nodes:
        if node[0] == 0 and len(temp):
            final_nodes.append(temp)
            temp = []
        temp.append(node)
    if len(temp):
        final_nodes.append(temp)
 
    print(len(final_nodes))
    tree_num = 0
    curr_id = sys.maxint
    flag = 0
    for node in leaf_nodes:
        if flag == 1:
            tree_num += 1
            flag = 0
        if node[0] == 0:
            final_nodes.insert(tree_num, [])
            final_nodes[tree_num].append(node)
            tree_num += 1
        else:
            if node[0] == end_leaf_nodes[tree_num]:
                flag = 1
            final_nodes[tree_num].append(node)

    sorted_final_nodes = []
    for tree in final_nodes:
        new_tree = sorted(tree, key=operator.itemgetter(0))
        sorted_final_nodes.extend(new_tree)

    return sorted_final_nodes


X, y = load_csv(data_filename) 
dtrain = xgb.DMatrix(X, label=y)
dump_svmlight_file(X, y, '/data9/dtrain.svm', zero_based=True)
dtrain_svm = xgb.DMatrix('/data9/dtrain.svm')

param = {
    'max_depth': 10,  # the maximum depth of each tree
    'eta': 0.3,  # the training step for each iteration
    'silent': 1,  # logging mode - quiet
    'objective': 'multi:softprob',  # error evaluation for multiclass training
    'num_class': 10}  # the number of classes that exist in this datset
num_round = 2048  # the number of training iterations

bst = xgb.train(param, dtrain, num_round)
preds = bst.predict(dtrain)
wrong = 0
for num, i in enumerate(preds):
    if np.argmax(i) != y[num]:
            wrong+=1
print("wrong: ")
print(wrong)
param['num_tree'] = num_round
bst.dump_model('/data9/xgb_model.txt', with_stats=True)

with open('/data9/xgb_model.txt', 'r') as f:
    txt_model = f.read()

ensemble = unpackNodes(txt_model)
ensemble.insert(0, param)
with open(json_filename, 'w') as f:
    f.write(json.dumps(ensemble))
