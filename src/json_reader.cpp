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
void JSONReader<T, F>::removeRegLeafNodes(std::vector<std::vector<StatNode<T, F>>> &bins, 
        std::vector<std::vector<StatNode<T, F>>> temp_ensemble ){
    std::vector<std::map<int, int>> id_to_index_vec;
    std::map<int, int> id_to_index;
    std::vector<std::map<int, int>> oldidx_to_newidx_vec;
    std::map<int, int> oldidx_to_newidx;
    std::vector<StatNode<T, F>> temp_bin;
    int node_counter = 0;
    int bin_size = 0;
    int left, right, newleft, newright;
    int num_bins = temp_ensemble.size();

    for(int i=0; i<num_bins; ++i){
        bin_size = temp_ensemble[i].size();
        //placeholder leaf node
        temp_bin.push_back(temp_ensemble[i][0]);
        for(int j=1; j<bin_size; ++j){
            if(temp_ensemble[i][j].getLeft() != -1 && temp_ensemble[i][j].getRight() != -1){
                if(temp_ensemble[i][temp_ensemble[i][j].getLeft()].getLeft() == -1){
                    //set to leaf node
                    temp_ensemble[i][j].setLeft(0);
                }
                if(temp_ensemble[i][temp_ensemble[i][j].getRight()].getLeft() == -1){
                    //set to leaf node
                    temp_ensemble[i][j].setRight(0);
                }
                id_to_index[temp_ensemble[i][j].getID()] = temp_bin.size();
                temp_bin.push_back(temp_ensemble[i][j]);
                oldidx_to_newidx[j] = temp_bin.size()-1 ;
            }
        }

        id_to_index_vec.push_back(id_to_index);
        id_to_index.clear();
        oldidx_to_newidx_vec.push_back(oldidx_to_newidx);
        oldidx_to_newidx.clear();
        bins.push_back(temp_bin);
        temp_bin.clear();
    }

    int nbins = bins.size();
    for(int i=0; i<nbins; ++i){
        for(int j=1; j<bins[i].size(); ++j){
            left = bins[i][j].getLeft();
            right = bins[i][j].getRight();
            if(left >= 1){
                newleft = id_to_index_vec[i][left];
                bins[i][j].setLeft(newleft);
            }
            if(right >= 1){
                newright = id_to_index_vec[i][right];
                bins[i][j].setRight(newright);
            }
        }
    }
}

template<typename T, typename F>
void JSONReader<T, F>::removeClassLeafNodes(std::vector<std::vector<StatNode<T, F>>> &bins, 
        std::vector<std::vector<StatNode<T, F>>> temp_ensemble ){
    int classnum = 0;
    std::vector<std::map<int, int>> id_to_index_vec;
    std::map<int, int> id_to_index;
    std::vector<std::map<int, int>> oldidx_to_newidx_vec;
    std::map<int, int> oldidx_to_newidx;
    std::vector<StatNode<T, F>> temp_bin;
    int num_classes = std::stoi(Config::getValue("numclasses"));
    int node_counter = 0;
    int bin_size = 0;
    int classn, left, right, newleft, newright;
    int num_bins = temp_ensemble.size();

    for(int i=0; i<num_bins; ++i){
        bin_size = temp_ensemble[i].size();
        for(int j=0; j<num_classes; ++j){
            temp_bin.push_back(temp_ensemble[i][j]);
        }
        for(int j=num_classes; j<bin_size; ++j){
            if(temp_ensemble[i][j].getLeft() != -1 && temp_ensemble[i][j].getRight() != -1){
                if(temp_ensemble[i][temp_ensemble[i][j].getLeft()].getLeft() == -1){
                    classn = temp_ensemble[i][temp_ensemble[i][j].getLeft()].getFeature();
                    temp_ensemble[i][j].setLeft(classn);
                }
                if(temp_ensemble[i][temp_ensemble[i][j].getRight()].getLeft() == -1){
                    classn = temp_ensemble[i][temp_ensemble[i][j].getRight()].getFeature();
                    temp_ensemble[i][j].setRight(classn);
                }
                id_to_index[temp_ensemble[i][j].getID()] = temp_bin.size();
                temp_bin.push_back(temp_ensemble[i][j]);
                oldidx_to_newidx[j] = temp_bin.size()-1 ;
            }
        }

        id_to_index_vec.push_back(id_to_index);
        id_to_index.clear();
        oldidx_to_newidx_vec.push_back(oldidx_to_newidx);
        oldidx_to_newidx.clear();
        bins.push_back(temp_bin);
        temp_bin.clear();
    }

    int nbins = bins.size();
    for(int i=0; i<nbins; ++i){
        for(int j=num_classes; j<bins[i].size(); ++j){
            left = bins[i][j].getLeft();
            right = bins[i][j].getRight();
            if(left >= num_classes){
                newleft = id_to_index_vec[i][left];
                bins[i][j].setLeft(newleft);
            }
            if(right >= num_classes){
                newright = id_to_index_vec[i][right];
                bins[i][j].setRight(newright);
            }
        }
    }
}

template<typename T, typename F>
void JSONReader<T, F>::convertSklToBins(std::vector<std::vector<StatNode<T, F>>> &bins, 
        std::vector<int> &bin_sizes, 
        std::vector<std::vector<int>> &bin_start,
        std::vector<int> &bin_node_sizes){

    std::string task = Config::getValue("task");
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
    int num_bins = populateBinSizes(bin_sizes, num_trees);

    //reserve memory for bins
    bins.reserve(num_bins);
    for(int i=0; i<num_bins; ++i)
        bins[i].reserve(bin_sizes[i]);

    //Recursively walk through the json model until we get the nodes per estimator
    int tree_offset = 0, bin_number = 0, tree_num_in_bin = 0;
    std::vector<StatNode<T, F>> temp_bin;

    if (task.compare(std::string("classification")) == 0) {
        int num_classes = (int) rf_json_model["estimators_"].at(0)["n_classes_"];
        Config::setConfigItem("numclasses", std::to_string(num_classes));
        for(int i=0; i<num_classes; ++i)
            temp_bin.push_back(StatNode<T, F>(-1, i, -1, -1, -1, -1, -1));
    }
    else {
        Config::setConfigItem("numclasses", std::to_string(1));
        temp_bin.push_back(StatNode<T, F>(-1, 0, -1, -1, -1, -1, -1));
    }
    tree_offset = temp_bin.size();
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
    int max = 0;
    int max_idx;
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

                temp_bin.emplace_back(left + tree_offset , right + tree_offset, 
                        feature, threshold, cardinality, id, depth);
            }

            //This is a leaf node. Populate the feature attr with class
            else {
                if (task.compare(std::string("classification")) == 0) {
                    auto class_card_arr = values.at(node_counter).at(0);
                    class_num = 0;
                    max = -1;
                    for (auto ele: class_card_arr){
                        if((int)ele > max){
                            max = (int)ele;
                            max_idx = class_num;
                        }
                        ++class_num;
                    }
                    feature = max_idx;
                    id = temp_bin.size();
                    temp_bin.emplace_back(left, right, feature, threshold, cardinality, id, 1);
                }
                //in case of regression, there are no "classes"
                else {
                    feature = node.at(2);
                    temp_bin.emplace_back(left, right, feature, threshold, cardinality, id, 1);
                }
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
            if (task.compare(std::string("classification")) == 0) {
                
                for(int i=0; i< std::stoi(Config::getValue("numclasses")); ++i)
                    temp_bin.push_back(StatNode<T, F>(-1, i, -1, -1, -1, -1, -1));
            }
            else {
                    temp_bin.push_back(StatNode<T, F>(-1, 0, -1, -1, -1, -1, -1));
            }
        }
        tree_offset = temp_bin.size();
    }

    if (task.compare(std::string("classification")) == 0)
        removeClassLeafNodes(bins, temp_ensemble );
    else
        removeRegLeafNodes(bins, temp_ensemble);

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
    ifs.close();
}
template class JSONReader<float, float>;
template class JSONReader<int, float>;
