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
    int populateBinSizes(std::vector<int>&bin_sizes, int num_trees);
    void removeLeafNodes(std::vector<std::vector<StatNode<T, F>>> &bins,
        std::vector<std::vector<StatNode<T, F>>> temp_ensemble);
    public:
        JSONReader(): filename(Config::getValue("modelfilename")), 
            package_name(Config::getValue("package")){}

        void convertToBins(std::vector<std::vector<StatNode<T, F>>>&bins, 
                std::vector<int>&bin_sizes, 
                std::vector<std::vector<int>>&bin_start,
                std::vector<int>&bin_node_sizes); 
};

#endif
