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
    //std::string model_filename = Config::getValue("modelfilename");
    std::ifstream ifs("/root/pacset/models/rf.json");
    json o = json::parse(ifs); 
    int count;
    json temp2;
    for (auto i: o["estimators_"]){
        json temp = json::parse(i.dump());
        count = 0;
        for(auto j: temp){
            if(count == parse_num){
                temp2 = json::parse(j.dump());
                auto a = temp2["nodes"];
                for (auto b: a){
                    for(auto c: b){
                        std::cout<<c<<"\n";
                    }
                }
                break;
            }
            ++count;
        }
        break;
    }
}
