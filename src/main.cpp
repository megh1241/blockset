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
    std::cout<<"Usage: ./exe --mode <inference/pack/both>"
        "--modelfilename <picklefilename>"
        "--datafilename <datafilename>"
        "--package <sklearn/lightgbm/xgboost/ranger>"
        "--algorithm <gbm/randomforest>"
        "--task <classification/regression>"
        "--numthreads <number of threads>\n";
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

    //TODO: change to num of cores

    PacsetFactory pf = PacsetFactory();
    
    std::cout<<"point 1\n";
    fflush(stdout);

    Config::setConfigItem("numthreads", std::string("3"));
    PacsetRandomForestClassifier<float, float>* obj = (PacsetRandomForestClassifier<float, float> *)pf.getModel<float, float>();
    std::cout<<"point 2\n";
    fflush(stdout);
    //PacsetRandomForestClassifier<float, float> *obj ;
    std::cout<<"point 3\n";
    fflush(stdout);
    //obj = dynamic_cast<PacsetRandomForestClassifier<float, float> *>(model);
    std::cout<<"point 4\n";
    fflush(stdout);
    //Read the model from file, pack and save to file
    obj->loadModel();
    std::cout<<"point 5\n";
    fflush(stdout);
    std::vector<std::vector<float>> test_vec;
    std::cout<<"point 6\n";
    fflush(stdout);
    std::vector<int> preds;
    loadTestData(test_vec); 
    std::cout<<"point 7\n";
    fflush(stdout);
    //Perform prediction
    obj->predict(test_vec, preds);
    std::cout<<"point 8\n";
    fflush(stdout);
    /*
    if (Config::getValue("numthreads") == std::string("notfound"))
    if(Config::getValue("mode") == std::string("pack") ||
            Config::getValue("mode") == std::string("both")){
       
        //auto model = pf.getModel<float, float>();
        //Read the model from file, pack and save to file
        //obj->loadModel();
    }
    
    std::string mode_string = Config::getValue(std::string("mode"));
    std::string inf_string = std::string("inference");
    std::string both_string = std::string("both");
    if( (mode_string.compare(inf_string) == 0) || 
            (mode_string.compare(both_string) == 0) ){
        std::vector<std::vector<float>> test_vec;
        std::vector<int> preds;
        loadTestData(test_vec); 
        //Perform prediction
        obj->predict(test_vec, preds);
        //delete obj;
    }
    */
}
