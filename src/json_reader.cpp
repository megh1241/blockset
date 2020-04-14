#include "config.h"
#include "stat_node.h"
#include "json_reader.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "config.h"

using json = nlohmann::json;

template<typename T, typename F>
int JSONReader<T, F>::populateBinSizes( 
        std::vector<int> &bin_sizes, int num_trees){

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

    return num_bins;
}


template<typename T, typename F>
void JSONReader<T, F>::removeLeafNodes(std::vector<std::vector<StatNode<T, F>>> &bins, 
        std::vector<std::vector<StatNode<T, F>>> temp_ensemble){
    int classnum = 0;
    std::vector<std::map<int, int>> id_to_index_vec;
    std::map<int, int> id_to_index;
    std::vector<StatNode<T, F>> temp_bin;
    int num_classes = std::stoi(Config::getValue("numclasses"));
    for(auto bin: temp_ensemble) {
        for(auto node: bin){
            //If the current node's left child is a leaf
            if(bin[node.getLeft()].getLeft() == -1){
                classnum = bin[node.getLeft()].getFeature();
                bin[node.getLeft()].setRight(classnum);
            }
            //If the current node's right child is a leaf
            if(bin[node.getRight()].getLeft() == -1){
                classnum = bin[node.getRight()].getFeature();
                bin[node.getRight()].setRight(classnum);
            }
            //If the current node is not a leaf, append it to the actual bin
            if(node.getLeft() != -1){
                id_to_index[node.getID()] = temp_bin.size();
                temp_bin.push_back(node);
            }
        }
        id_to_index_vec.push_back(id_to_index);
        id_to_index.clear();
        bins.push_back(temp_bin);
        temp_bin.clear();
    }

    int node_count = 0, bin_count = 0, left = 0, right = 0;
    for(auto &bin : bins){
        for(auto &node: bin){
            if(node_count < num_classes)
                continue;
            left = node.getLeft();
            right = node.getRight();
            if(node.getID() > -1){
                if(node.getLeft() > -1){
                    node.setLeft(id_to_index_vec[bin_count][bin[left].getID()]);
                    node.setRight(id_to_index_vec[bin_count][bin[right].getID()]);
                }
            }
            node_count++;
        }
        node_count = 0;
        ++bin_count;
    }
}

template<typename T, typename F>
void JSONReader<T, F>::convertToBins(std::vector<std::vector<StatNode<T, F>>> &bins, 
        std::vector<int> &bin_sizes, 
        std::vector<std::vector<int>> &bin_start,
        std::vector<int> &bin_node_sizes){
    const int parse_num = 8; 
    int count;

    std::string model_filename = Config::getValue("modelfilename");
    std::ifstream ifs(model_filename);
    json rf_json_model = json::parse(ifs); 
    json node_json;

    int num_classes = (int) rf_json_model["estimators_"].at(0)["n_classes_"];
    Config::setConfigItem("numclasses", std::to_string(num_classes));

    //Get the number of trees
    auto meta_params = rf_json_model["params"];
    json param_j = json::parse(meta_params.dump());
    int num_trees = param_j["n_estimators"];

    int num_bins = populateBinSizes(bin_sizes, num_trees);

    //reserve memory for bins
    bins.reserve(num_bins);
    for(int i=0; i<num_bins; ++i)
        bins[i].reserve(bin_sizes[i]);

    //Recursively walk through the json model until we get the nodes per estimator
    int tree_offset = 0, bin_number = 0, tree_num_in_bin = 0;
    std::vector<StatNode<T, F>> temp_bin;

    for(int i=0; i<num_classes; ++i)
        temp_bin.push_back(StatNode<T, F>(-1, i, -1, -1, -1, -1, -1));

    //Note: temp_ensemble contains the leaf nodes as a separate node.
    //We want the leafs to point to the class nodes. temp_ensemble doesnot
    //contain class nodes.

    std::vector<std::vector<StatNode<T, F>>> temp_ensemble;
    int left=0, right=0, cardinality=0, feature=0;
    int id = 0;
    int node_counter = 0;
    int class_num = 0;
    float threshold = 0;
    int depth = 1;
    std::vector<int> tree_starts;

    for (auto tree: rf_json_model["estimators_"]){
        json tree_estimator = json::parse(tree.dump());
        auto nodes = tree_estimator["tree_"]["nodes"];
        auto values = tree_estimator["tree_"]["values"];
        for (auto node: nodes){
            left = node.at(0);
            right = node.at(1);
            threshold = node.at(3);
            cardinality = node.at(5);

            //Internal node
            if(left > -1){
                feature = node.at(2);
                id = temp_bin.size();
                if(left == 1)
                    depth = 0;
                else
                    depth = 1;
                
                temp_bin.emplace_back(left + tree_offset, right + tree_offset, 
                        feature, threshold, cardinality, id, depth);
            }

            //This is a leaf node. Populate the feature attr with class
            else {
                auto class_card_arr = values.at(node_counter).at(0);
                class_num = 0;
                for (auto ele: class_card_arr){
                    if((int)ele > 0){
                        feature = class_num;
                        break;
                    }
                    ++class_num;
                }
                id = temp_bin.size();
                temp_bin.emplace_back(left, right, feature, threshold, cardinality, id, 1);
            }
            ++node_counter;
        }
        node_counter = 0;
        ++tree_num_in_bin;

        //Move on to the next bin if the current bin reached full capacity
        if(tree_num_in_bin == bin_sizes[bin_number]){
            ++bin_number;
            tree_num_in_bin = 0;
            temp_ensemble.push_back(temp_bin); 
            temp_bin.clear();
            for(int i=0; i<num_classes; ++i)
                temp_bin.push_back(StatNode<T, F>(-1, i, -1, -1, -1, -1, -1));
        }
        tree_offset = temp_bin.size();
    }
    removeLeafNodes(bins, temp_ensemble);
    
    //populate the tree root indices (bin_start)
    //TODO: this is inefficient, do this check in removeLeafNodes!
    int counter = 0;
    for(auto bin: bins){
        for(auto node: bin){
            //root node
            if(node.getDepth() == 0)
                tree_starts.push_back(counter);
            
            counter++;
        }
        counter = 0;
        bin_node_sizes.push_back(bin.size());
        bin_start.push_back(tree_starts);
        tree_starts.clear();
    }
    /*
    std::cout<<"Printing starts: \n";
    for(auto i: bin_start){
        for(auto j: i){
            std::cout<<j<<"\n";
        }
        std::cout<<"**************************\n";
    }
    std::cout<<"Printing sizes: \n";
    for(auto i: bin_sizes){
            std::cout<<i<<"\n";
    }
    */
}
template class JSONReader<float, float>;
template class JSONReader<int, float>;
