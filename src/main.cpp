// main.cpp

#include <iostream>
#include <string>
#include <cstring>
#include <unordered_set>
#include "config.h"

//TODO: Python Bindings
//TODO: Separate out into include dir
//TODO: Namespace ?
//TODO: setup Testing framework google test?

static void show_usage(){
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

    if (argc < 6){
        std::cerr<<"Invalid set of commandline args!\n";
        show_usage();
        exit(-1);
    }

    for(int i=1; i<argc; i+=2){
        if(strcmp(argv[i], "--help") == 0)
            show_usage();
        else if(cmdline_args.find(argv[i]) != cmdline_args.end()){
            std::string config_key = std::string(argv[i]).erase(0, 2);
            Config::setConfigItem(config_key, argv[i+1]);  
        }
        else{
            std::cerr<<"Unknown commandline args: "<<argv[i]<<"\n";
            show_usage();
            exit(-1);
        }
        //TODO: check legality of the args
    }
    
}
