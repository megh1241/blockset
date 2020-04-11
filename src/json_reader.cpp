#include "config.h"
#include "stat_node.h"
#include "json_reader.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "config.h"

using json = nlohmann::json;

void JSONReader::convertToBins(std::vector<std::vector<StatNode<float, float>>> &bins){
    const int parse_num = 8; 
    int count;
    std::string model_filename = Config::getValue("modelfilename");
    std::ifstream ifs(model_filename);
    json rf_json_model = json::parse(ifs); 
    json node_json;
   
    //Get the number of trees
    auto meta_params = rf_json_model["params"];
    json param_j = json::parse(meta_params.dump());
    int n_trees = param_j["n_estimators"];
    
    //Calculate Bin sizes from number of trees
    int num_bins = std::stoi(Config::getValue("numthreads")); 

    //Recursively walk through the json model until we get the nodes per estimator
    for (auto ele: rf_json_model["estimators_"]){
        json estimator = json::parse(ele.dump());
        count = 0;

        for(auto item: estimator){
            if(count == parse_num){
                node_json = json::parse(item.dump());
                auto nodes = node_json["nodes"];
                for (auto node: nodes){
                    for(auto attr: node){
                        int a = 1;
                        //std::cout<<attr<<"\n";
                    }
                }
                break;
            }
            ++count;
        }
    }
}
