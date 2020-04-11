#include "config.h"
#include "stat_node.h"
#include "json_reader.h"
#include <vector>
#include <iostream>
#include <fstream>

using json = nlohmann::json;
void JSONReader::convertToBins(std::vector<std::vector<StatNode<float, float>>> &bins){
    std::ifstream ifs("/root/pacset/models/rf.json");
    json o = json::parse(ifs); 
    //each estimator
    int count = 0;
    json temp2;
    for (auto i: o["estimators_"]){
        json temp = json::parse(i.dump());
        count = 0;
        for(auto j: temp){
            if(count == 8){
                temp2 = json::parse(j.dump());
            }
            ++count;
        }
        std::cout<<temp2<<"\n";
        break;
    }

}
    
