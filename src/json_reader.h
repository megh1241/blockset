#ifndef JSON_R
#define JSON_R

#include "config.h"
#include "stat_node.h"
#include"../jsonlib/json.hpp"
#include <vector>

class JSONReader{
    std::string filename;
    std::string package_name;
    public:
        JSONReader(): filename(Config::getValue("modelfilename")), package_name(Config::getValue("package")){}
        std::vector<std::vector<StatNode<float, float>>> convertToBins();
};






#endif
