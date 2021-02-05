# PACSET

This repository contains the source code and experimental workflow accompanying our KDD 2021 manuscript. 
Detailed instructions are given below.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites
This source code has only been tested on Ubuntu 18.04.3 LTS, though we expect it to work on other
Operating Systems with minor changes. In the near future, we plan to construct python bindings and release a pip installable
python package.

1)You will need to install Build tools, openMP and make sure your compiler supports C++14. 
2)You will also need to install sklearn, numpy, matplotlib and joypy(for plotting).
3)PACSET utilizes a third-party library( rapidjson ) and must be downloaded and installed.

```
sudo apt-get update
sudo apt-get install build-essential
pip3 install numpy sklearn matplotlib
pip install joypy
```

### Installing
```

cd pacset
git clone https://github.com/Tencent/rapidjson.git
mkdir build
cd build
cmake .
make #This will generate the exe executable
```

### Running

#### For embedded PACSET and larger than RAM PACSET, the workflow is as follows.
1) Run train\_sklearn.sh to train and save a random forest/gradient boosted tree model in scikitlearn (or xgboost). Refer to train\_sklearn.sh for specific
instruction for how to run it and parameter explanations. Specifically, look for lines with #TODO that contain instructions for the user to modify parameters. 
2) Run pack.sh to pack the model using PACSET's packing layout. This script will read the sklearn trained model, pack the nodes and finally save the model in 
a custom binary format.
3) Run cold\_start\_inference.sh to perform RF/GBT inference on the test data and run the latency benchmarks. In order to ensure a cold-start latency in our benchmarks, 
we create identical copies of the model. We then perform inference on a different copy of the model per data point in a cyclical manner. That way, caching effects are eliminated.

#### For PACSET-as-service, the workflow is as follows.
1) Run train\_sklearn.sh to train and save a random forest/gradient boosted tree model in scikitlearn (or xgboost). Refer to train\_sklearn.sh for specific
instruction for how to run it and parameter explanations. Specifically, look for lines with #TODO that contain instructions for the user to modify parameters.
2) Run pack.sh to pack the model using PACSET's packing layout. This script will read the sklearn trained model, pack the nodes and save the model. Make sure to change
format to "text" in the script to ensure that the model is saved in a text format.
3)
4)

#### For the baseline experiments(comparison with xgboost and sklearn), the workflow is as follows.
