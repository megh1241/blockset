// main.cpp

#include <iostream>
#include <string>
#include <cstring>
#include <unordered_set>
#include "config.h"
#include "pacset_factory.h"
#include "pacset_base_model.h"
#include "pacset_rf_classifier.h"
#include "pacset_factory.cpp"

//TODO: Python Bindings
//TODO: Separate out into include dir
//TODO: Namespace ?
//TODO: setup Testing framework google test?
//TODO: Allow the user to specify datatypes for feature and threshold ? 

const int min_num_cmd_args = 6;

static void showUsage(){
    std::cout<<"Usage: ./exe --mode <inference/pack>"
        "--modelfilename <picklefilename>"
        "--datafilename <datafilename>"
        "--package <sklearn/lightgbm/xgboost/ranger>"
        "--algorithm <gbm/randomforest>"
        "--task <classification/regression>\n";
}

int main(int argc, char* argv[]) {
    
    const std::unordered_set<std::string> cmdline_args = {"--help", "--mode", 
        "--datafilename", "--modelfilename", "--package", 
        "--algorithm", "--task"};

    if (argc < min_num_cmd_args){
        std::cerr<<"Invalid set of commandline args!\n";
        showUsage();
        exit(-1);
    }

    for(int i=1; i<argc; i+=2){
        if(strcmp(argv[i], "--help") == 0)
            showUsage();
        else if(cmdline_args.find(argv[i]) != cmdline_args.end()){
            std::string config_key = std::string(argv[i]).erase(0, 2);
            Config::setConfigItem(config_key, argv[i+1]);  
        }
        else{
            std::cerr<<"Unknown commandline args: "<<argv[i]<<"\n";
            showUsage();
            exit(-1);
        }
        //TODO: check legality of the combination of args
    }
    PacsetFactory pf = PacsetFactory();
    auto model = pf.getModel<float, float>();
    PacsetRandomForestClassifier<float, float> *obj = dynamic_cast<PacsetRandomForestClassifier<float, float> *>(model);
    obj->loadModel();
}

