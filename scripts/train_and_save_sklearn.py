from sklearn.ensemble import RandomForestClassifier
from sklearn.datasets import load_iris
from sklearn.tree import DecisionTreeClassifier
import pickle
import sklearn_json as skljson

######## GLOBALS #########
n_trees = 10

###### BEGIN SCRIPT ######

#Load data
iris = load_iris()
X = iris.data
y = iris.target

#Train model
#model = RandomForestClassifier(n_estimators = n_trees)
model = DecisionTreeClassifier()

model.fit(X, y)

#Save model to pickle
pickle.dump(model, open('../models/dtree.pkl', 'wb'))

#Save model to json
skljson.to_json(model, '../models/dtree.json')


