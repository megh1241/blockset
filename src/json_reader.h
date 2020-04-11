#ifndef JSON_R
#define JSON_R

#include "config.h"
#include "stat_node.h"

#include <vector>
#include <nlohmann/json.hpp>

// for convenience
using json = nlohmann::json;

class JSONReader{
    std::string filename;
    std::string package_name;
    public:
        JSONReader(): filename(Config::getValue("modelfilename")), package_name(Config::getValue("package")){}
        void convertToBins(std::vector<std::vector<StatNode<float, float>>>&bins, std::vector<int>&bin_sizes);
};






#endif
