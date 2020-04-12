#include "config.h"
#include "stat_node.h"
#include "json_reader.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "config.h"

using json = nlohmann::json;

void JSONReader::convertToBins(std::vector<std::vector<StatNode<float, float>>> &bins, std::vector<int> &bin_sizes){
    const int parse_num = 8; 
    int count;
    std::string model_filename = Config::getValue("modelfilename");
    std::ifstream ifs(model_filename);
    json rf_json_model = json::parse(ifs); 
    json node_json;

    //Get the number of trees
    auto meta_params = rf_json_model["params"];
    json param_j = json::parse(meta_params.dump());
    int num_trees = param_j["n_estimators"];

    //Calculate Bin sizes from number of trees
    int num_bins = std::stoi(Config::getValue("numthreads")); 
    int num_trees_per_bin = num_trees / num_bins;

    //populate bin_sizes
    for(int i=0; i<num_bins; ++i)
        bin_sizes.push_back(num_trees_per_bin);

    //Add remaining trees to bin_sizes
    int leftover_trees = num_trees % num_bins;
    for(int i=0; i<leftover_trees; ++i)
        bin_sizes[i % num_bins]++;

    //reserve memory for bins
    bins.reserve(num_bins);
    for(int i=0; i<num_bins; ++i)
        bins[i].reserve(bin_sizes[i]);

    //Recursively walk through the json model until we get the nodes per estimator
    int tree_offset = 0, bin_number = 0, tree_num_in_bin = 0;
    std::vector<StatNode<float,float>> temp_bin;
    
    //Note: temp_ensemble contains the leaf nodes as a separate node.
    //We want the leafs to point to the class nodes. temp_ensemble doesnot
    //contain class nodes.
    
    std::vector<std::vector<StatNode<float, float>>> temp_ensemble;
    for (auto ele: rf_json_model["estimators_"]){
        json estimator = json::parse(ele.dump());
        count = 0;
        int bin_node_count = 0;
        for(auto item: estimator){
            if(count == parse_num){
                node_json = json::parse(item.dump());
                auto nodes = node_json["nodes"];
                for (auto node: nodes){
                    //TODO: create a statNode and append it to bins
                    int attr_index = 0, left = 0, right = 0, feature = 0, cardinality = 0;
                    float threshold = 0.0;
                    for (auto attr: node){
                        switch(attr_index){
                            case 0:
                                left = attr;
                                break;
                            case 1:
                                right = attr;
                                break;
                            case 2:
                                feature = attr;
                                break;
                            case 3:
                                threshold = attr;
                                break;
                            case 5:
                                cardinality = attr;
                                break;
                            default:
                                break;
                        }
                        attr_index++;
                    }
                    //TODO: remove
                    if(left > -1)
                        temp_bin.emplace_back(left + tree_offset, right + tree_offset, feature, threshold, cardinality);
                    else
                        temp_bin.emplace_back(left, right, feature, threshold, cardinality);
                    
                    bin_node_count++;
                }
                break;
            }
            ++count;
        }
        ++tree_num_in_bin;

        //Move on to the next bin if the current bin reached full capacity
        if(tree_num_in_bin == bin_sizes[bin_number]){
            ++bin_number;
            tree_num_in_bin = 0;
            temp_ensemble.push_back(temp_bin); 
            temp_bin.clear();
            //temp_bin.reserve(bin_sizes[bin_number]);
            bin_node_count = 0;
        }
        tree_offset = temp_bin.size();
    }

    for(auto i: temp_ensemble){
        std::cout<<"NEW BIN*********************\n";
        for(auto node: i){
            node.printNode();
        }
    }
}
