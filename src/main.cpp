// main.cpp

#include <iostream>
#include <string>
#include <cstring>
#include <unordered_set>
#include <omp.h>
#include "config.h"
#include "pacset_factory.h"
#include "pacset_base_model.h"
#include "pacset_rf_classifier.h"
#include "pacset_factory.cpp"
#include "utils.h"

//TODO: Python Bindings
//TODO: Separate out into include dir
//TODO: Namespace ?
//TODO: setup Testing framework google test?
//TODO: Allow the user to specify datatypes for feature and threshold ? 
//TODO: categorical features ?
//TODO: convert classes to ints
//TODO: figure out template stuff


const int min_num_cmd_args = 6;

static void showUsage(){
    std::cout<<"Usage: "
        "./exe --mode <inference/pack/both>"
        "--modelfilename <picklefilename>"
        "--datafilename <datafilename>"
        "--package <sklearn/lightgbm/xgboost/ranger>"
        "--algorithm <gbm/randomforest>"
        "--task <classification/regression>"
        "--numthreads <number of threads>\n";
}

static void parseArgs(int argc, char* argv[]){
    //TODO: check legality of the combination of args
    const std::unordered_set<std::string> cmdline_args =
        {"--help", "--mode", "--layout", "--intertwine", 
        "--datafilename", "--modelfilename", "--package", 
        "--algorithm", "--task", "--numthreads"};

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
    }

    //Set number of threads openmp
    if(Config::getValue("numthreads") != std::string("notfound")){
        omp_set_dynamic(0);
        omp_set_num_threads(std::atoi(Config::getValue("numthreads").c_str()));
    }
}

int main(int argc, char* argv[]) {
    parseArgs(argc, argv);

    std::vector<int> preds;
    std::vector<int> lab;
    std::vector<int> predi;    
    PacsetFactory pf = PacsetFactory();

    PacsetBaseModel<float, float> *obj;
    if(Config::getValue("algorithm") == std::string("randomforest") 
            && Config::getValue("task") == std::string("classification")) {
        obj = (PacsetRandomForestClassifier<float, float> *)
            pf.getModel<float, float>();
    }

    //Read the model from file, pack and save to file
    if(Config::getValue("mode") == std::string("both")){
        //load json model from disk    
        obj->loadModel();
        std::cout<<"model loaded\n";
        //pack model
        obj->pack();
        std::cout<<"model packed \n";
        //Load test data from file
        std::vector<std::vector<float>> test_vec;
        loadTestData(test_vec, lab); 
        std::cout<<"test data loaded\n";
        //Perform prediction
        obj->predict(test_vec, preds, predi);
        std::cout<<"predicted\n"; 
        //Compute accuracy
        double acc = getAccuracy(predi, lab);
        std::cout<<"Accuracy: "<<acc<<"\n";
        
        //save packed model to file
        obj->serialize();
   
    }
    else if (Config::getValue("mode") == std::string("pack")){
        
        //Read the model from file, pack and save to file
        obj->loadModel();

        //pack model
        obj->pack();

        //save packed model to file
        obj->serialize();
    }
    else if (Config::getValue("mode") == std::string("inference")){
        //TODO: fill
        obj->deserialize();
    }
}
