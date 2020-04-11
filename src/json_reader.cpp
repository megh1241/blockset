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
    //std::string model_filename = Config::getValue("modelfilename");
    std::ifstream ifs("/root/pacset/models/rf.json");
    json rf_json_model = json::parse(ifs); 
    json node_json;

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
                        std::cout<<attr<<"\n";
                    }
                }
                break;
            }
            ++count;
        }
        break;
    }
}
