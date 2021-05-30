#include <iostream>
#include <string>
#include <cstring>

#include "pacset_base.h"


void PacsetBase::initRandomForestClassifier(){
        obj = new PacsetRandomForestClassifier<float, float>();
}
void PacsetBase::initRandomForestRegressor(){
        obj = new PacsetRandomForestRegressor<float, float>();
}
void PacsetBase::initGradientBoostedClassifier(){
        obj = new PacsetGradientBoostedClassifier<float, float>();
}
void PacsetBase::initGradientBoostedRegressor(){
        obj = new PacsetGradientBoostedRegressor<float, float>();
}

void PacsetBase::loadAndPack(){
	//Read the model from file, pack and save to file
        obj->loadModel();
        //pack model
        obj->pack();
        //save packed model to file
        obj->serialize();
}

void PacsetBase::predict(){
        std::vector<std::vector<float>> test_vec;    
        obj->deserialize();
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
        //Load test data from file
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
