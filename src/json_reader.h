#ifndef JSON_R
#define JSON_R

#include "config.h"
#include "stat_node.h"

#include <vector>
#include <nlohmann/json.hpp>

// for convenience
using json = nlohmann::json;

template<typename T, typename F>
class JSONReader{
    std::string filename;
    std::string package_name;
    public:
        JSONReader(): filename(Config::getValue("modelfilename")), package_name(Config::getValue("package")){}
        void convertToBins(std::vector<std::vector<StatNode<T, F>>>&bins, std::vector<int>&bin_sizes);
};






#endif
