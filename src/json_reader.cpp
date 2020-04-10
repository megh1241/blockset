#include "config.h"
#include "stat_node.h"
#include "json_reader.h"
#include <vector>
#include <iostream>

using json = nlohmann::json;
void JSONReader::convertToBins(std::vector<std::vector<StatNode<float, float>>> &bins){
 json j_complete = json::parse("../models/rf.json");
 for(auto& element: j_complete)
     std::cout<<element<<"\n";
}

