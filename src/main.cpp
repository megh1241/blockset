// main.cppEE

#include <iostream>
#include <string>
#include <cstring>
#include <unordered_set>
#include <omp.h>
#include "config.h"
#include "pacset_factory.h"
#include "pacset_base_model.h"
#include "pacset_rf_classifier.h"
#include "pacset_rf_regressor.h"
#include "pacset_gb_classifier.h"
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
//TODO: Feature: Allow mmemory mapping of observations

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
        "--packfilename", "--datafilename", "--modelfilename2", 
        "--modelfilename", "--package","--algorithm", 
        "--task", "--numthreads", "--format",
    	"--metadatafilename", "--blocksize" };

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

    //set default block size
    if(Config::getValue("blocksize") == std::string("notfound")){
        Config::setConfigItem(std::string("blocksize"), std::string("2048"));
    }
    
    if(Config::getValue("format") == std::string("notfound")){
        Config::setConfigItem(std::string("format"), std::string("binary"));
    }
}

int main(int argc, char* argv[]) {
    parseArgs(argc, argv);

    PacsetFactory pf = PacsetFactory();

    PacsetBaseModel<float, float> *obj;
    if(Config::getValue("algorithm") == std::string("randomforest") 
            && Config::getValue("task") == std::string("classification")) {
        obj = (PacsetRandomForestClassifier<float, float> *)
            pf.getModel<float, float>();
    }
    else if(Config::getValue("algorithm") == std::string("randomforest") 
            && Config::getValue("task") == std::string("regression")) {
        obj = (PacsetRandomForestRegressor<float, float> *)
            pf.getModel<float, float>();
    }
    else if(Config::getValue("algorithm") == std::string("gradientboost")
            && Config::getValue("task") == std::string("classification")) {
        obj = (PacsetGradientBoostedClassifier<float, float> *)
            pf.getModel<float, float>();
    } 

    //Read the model from file, pack and save to file
    if(Config::getValue("mode") == std::string("both")){
        //load json model from disk    
	obj->loadModel();
	//pack model
        obj->pack();
        //Load test data from file
        std::vector<std::vector<float>> test_vec;
        if (Config::getValue("task") == std::string("classification")){
            std::vector<int> lab;
            std::vector<int> preds;
            std::vector<int> predi;    
            loadTestData(test_vec, lab); 
            //Perform prediction
            obj->predict(test_vec, preds, predi, false);
            
            //Compute accuracy
            double acc = getAccuracy(predi, lab);
            std::cout<<"Accuracy: "<<acc<<"\n";
        }
        else {
            //Perform prediction
            std::vector<double> preds;
            std::vector<double> predi;    
            std::vector<double> lab;
            loadTestData(test_vec, lab); 
            obj->predict(test_vec, preds, predi, false);
            
            double acc = getAccuracy(predi, lab);
            std::cout<<"Accuracy: "<<acc<<"\n";
        }
        
        //save packed model to file
        obj->serialize();

    }
    else if (Config::getValue("mode") == std::string("pack")){

        //Read the model from file, pack and save to file
        obj->loadModel();
	std::cout<<"Model loaded\n";
	fflush(stdout);
        //pack model
        obj->pack();

        //save packed model to file
        obj->serialize();
    }
    else if (Config::getValue("mode") == std::string("inference")){
        //TODO: fill
	    std::cout<<"before deserialize\n";
        	obj->deserialize();
	    std::cout<<"after deserialize\n";

        //Load test data from file
        std::vector<std::vector<float>> test_vec;


        if ((Config::getValue("algorithm") == std::string("randomforest")) && (Config::getValue("task") == std::string("classification"))){
            std::vector<int> preds;
            std::vector<int> predi;    
            std::vector<int> lab;
            //Perform prediction
	    std::cout<<"loading test data\b";
	    loadTestData(test_vec, lab); 
          	std::cout<<"test data loaded\n";
            obj->predict(test_vec, preds, predi, true);
            std::cout<<"predicted\n"; 
            //Compute accuracy
            std::cout<<"Size of predicted: "<<predi.size();
            for (auto i: predi)
                std::cout<<i<<", ";
            std::cout<<"\n";
            
            double acc = getAccuracy(predi, lab);
            std::cout<<"Accuracy: "<<acc<<"\n";
        }
        else{
	    if(Config::getValue("task") == std::string("regression")){
            	std::cout<<"Not classification!\n";
            	std::vector<double> preds;
            	std::vector<double> predi;    
            	std::vector<double> lab;
            	//Perform prediction
            	loadTestData(test_vec, lab); 
            	obj->predict(test_vec, preds, predi, true);
            	std::cout<<"predicted\n"; 
            	//Compute accuracy
            	double acc = getAccuracy(predi, lab);
            	std::cout<<"Accuracy: "<<acc<<"\n";
	    }
	    else{
            	std::vector<double> preds;
            	std::vector<double> pred_d;    
		std::vector<int> pred_i;
            	std::vector<int> lab;
            	//Perform prediction
            	loadTestData(test_vec, lab); 
            	obj->predict(test_vec, preds, pred_d, true);
		for(auto i: pred_d)
			pred_i.push_back((int)i);
            	std::cout<<"predicted\n"; 
            	double acc = getAccuracy(pred_i, lab);
            	std::cout<<"Accuracy: "<<acc<<"\n";
 	    
	    }
        }
    }
}
