#ifndef PACKER_H
#define PACKER_H

#include <iostream>
#include "stat_node.h"
#include "node.h"
#include "config.h"
#include <algorithm>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <cmath>

template <typename T, typename F>
class Packer{

    int depth_intertwined;
    std::string layout;
    std::vector<StatNode<T, F>> finalbin;
    std::map<int, int> node_to_index;
    std::map<int, int> subtree_count_map;

    private:
    //Note: contains helper functions (i.e common routines used for packing)

    //Returns a queue containing the roots of subtrees below the BIN
    //after packing the BIN in an interleaved fashion

    inline std::deque<StatNode<T, F>> packBinHelper(
            std::vector<StatNode<T, F>>&bin, 
            const int num_trees_in_bin, 
            std::vector<int> &bin_start) {

        int num_nodes_process = std::pow(2, depth_intertwined) - 1;
        int num_classes = std::atoi(Config::getValue("numclasses").c_str());
        std::deque <StatNode<T, F>> temp_q;
        std::deque <StatNode<T, F>> bin_q;
        std::deque <StatNode<T, F>> bin_q_left;
        std::deque <StatNode<T, F>> bin_q_right;

        for(int i=0; i<num_trees_in_bin; ++i){
            temp_q.push_back(bin[bin_start[i]]);
            //finalbin.push_back(bin[bin_start[i]]);
            //node_to_index.insert(std::pair<int, int>(bin[bin_start[i]].getID(), finalbin.size()-1));
            //trees are interleaved so the starting nodes are adjacent
            //but after the class nodes
            bin_start[i] = i+num_classes;
        }

        // Intertwined levels
        int curr_level = 0; 
        int pos_in_level = 0;

        while(curr_level < num_nodes_process * num_trees_in_bin) {
            auto ele = temp_q.front();
            ele.printNode();
            temp_q.pop_front();
            if(ele.getID()>= 0) {
                finalbin.push_back(ele);
                node_to_index.insert(std::pair<int, int>(ele.getID(), finalbin.size()-1));
                bin_q_left.push_back(bin[ele.getLeft()]); 
                bin_q_right.push_back(bin[ele.getRight()]); 
            }
            else {
                bin_q_left.push_back(bin[ele.getRight()]); 
                bin_q_right.push_back(bin[ele.getRight()]); 
            }

            if(pos_in_level == num_trees_in_bin - 1){
                while(!bin_q_left.empty()){
                    auto ele = bin_q_left.front();
                    temp_q.push_back(ele);
                    bin_q_left.pop_front();
                }
                while(!bin_q_right.empty()){
                    auto ele = bin_q_right.front();
                    temp_q.push_back(ele);
                    bin_q_right.pop_front();
                }
                pos_in_level = 0;
            }
            else
                pos_in_level++;

            curr_level++;
        }

        while(!temp_q.empty()){
            auto ele = temp_q.front();
            if(ele.getID() >= 0)
                bin_q.push_back(ele);
            temp_q.pop_front();
        }
        return bin_q;
    }

    inline void packSubtreeBFSHelper(
            std::vector<StatNode<T, F>>&bin, 
            const int num_trees_in_bin, 
            std::vector<int> &bin_start, 
            std::deque<StatNode<T, F>> &bin_q) { 

        int num_classes = std::atoi(Config::getValue("numclasses").c_str());
        while(!bin_q.empty()){
            std::deque<StatNode<T, F>> bin_st;
            auto ele = bin_q.front();
            bin_q.pop_front();
            bin_st.push_back(ele);
            while(!bin_st.empty()){
                auto ele = bin_st.front();
                bin_st.pop_front(); 
                finalbin.push_back(ele);
                node_to_index.insert(std::pair<int, int>(ele.getID(), finalbin.size()-1));
                if((ele.getLeft() < num_classes) && (ele.getRight() < num_classes))
                    continue;

                else if(ele.getLeft() < num_classes)
                    bin_st.push_back(bin[ele.getRight()]);

                else if(ele.getRight() < num_classes)
                    bin_st.push_back(bin[ele.getLeft()]); 

                else {
                    bin_st.push_back(bin[ele.getLeft()]); 
                    bin_st.push_back(bin[ele.getRight()]); 
                }
            }
        }		
    }

    inline void packSubtreeDFSHelper(
            std::vector<StatNode<T, F>>&bin, 
            const int num_trees_in_bin, 
            std::vector<int> &bin_start, 
            std::deque<StatNode<T, F>> &bin_q) { 

        int num_classes = std::atoi(Config::getValue("numclasses").c_str());
        while(!bin_q.empty()){
            std::deque<StatNode<T, F>> bin_st;
            auto ele = bin_q.front();
            bin_q.pop_front();
            bin_st.push_back(ele);
            while(!bin_st.empty()){
                auto ele = bin_st.front();
                bin_st.pop_front(); 
                finalbin.push_back(ele);
                node_to_index.insert(std::pair<int, int>(ele.getID(), 
                            finalbin.size()-1));
                if((ele.getLeft() < num_classes) && 
                        (ele.getRight() < num_classes))
                    continue;

                else if(ele.getLeft() < num_classes)
                    bin_st.push_front(bin[ele.getRight()]);

                else if(ele.getRight() < num_classes)
                    bin_st.push_front(bin[ele.getLeft()]); 

                else {
                    if(layout.find(std::string("stat")) != std::string::npos ||
                            layout.find(std::string("weighted")) != std::string::npos){

                        if(bin[ele.getLeft()].getCardinality() < 
                                bin[ele.getRight()].getCardinality()){
                            bin_st.push_front(bin[ele.getLeft()]); 
                            bin_st.push_front(bin[ele.getRight()]);
                        }
                        else{
                            bin_st.push_front(bin[ele.getRight()]);
                            bin_st.push_front(bin[ele.getLeft()]); 
                        }
                    }
                    else{
                        bin_st.push_front(bin[ele.getLeft()]); 
                        bin_st.push_front(bin[ele.getRight()]);
                    }
                }
            }
        }		
    }

    StatNode<T, F> popMaxCardEle(std::deque<StatNode<T, F>> &bin_st){
        int max = -1;
        int positer = 0;
        int ecount = 0;

        StatNode<T, F> ele;
        for(auto ii: bin_st) {
            if((ii.getCardinality() > max) || (ii.getCardinality() == max && ii.getDepth() < ele.getDepth())){
                max = ii.getCardinality();
                positer = ecount;
                ele = ii;
            }
            ecount++;
        }
        bin_st.erase(bin_st.begin() + positer, bin_st.begin() + positer +1);
        return ele;
    }
    
    
    inline bool myCompFunction(StatNode<T, F> &node1, StatNode<T, F> &node2){

        if(node1.getSTNum() == node2.getSTNum())
            return node1.getID() < node2.getID();

        if(subtree_count_map[node1.getSTNum()] == subtree_count_map[node2.getSTNum()])
            return node1.getSTNum() < node2.getSTNum();

        return subtree_count_map[node1.getSTNum()] > subtree_count_map[node2.getSTNum()];

    }

    inline std::deque<StatNode<T, F>>  packSubtreeBlockwiseClassHelper(
            std::vector<StatNode<T, F>>&bin, 
            const int num_trees_in_bin, 
            std::vector<int> &bin_start, 
            std::deque<StatNode<T, F>> &bin_q) { 
        int num_classes = std::atoi(Config::getValue("numclasses").c_str());
        int block_size = std::atoi(Config::getValue("blocksize").c_str());
        //int pos_in_block = (finalbin.size()-1) % block_size;
        
        //TODO: insert blank nodes
        
        int pos_in_block = 0;
        int subtree_count = 0;

        subtree_count_map[0] = 0;
        while(!bin_q.empty()){
            auto ele = bin_q.front();
            if(pos_in_block != block_size - 1){
                bin_q.pop_front();
                subtree_count_map[subtree_count]++;
            }
            else{
                subtree_count++;
                subtree_count_map[subtree_count] = 0;
                ele = popMaxCardEle(bin_q);
            }

            finalbin.push_back(ele);
            int fsiz = finalbin.size();
            finalbin[fsiz-1].setSubtreeNum(subtree_count);

            node_to_index.insert(std::pair<int, int>(ele.getID(), 
                        finalbin.size()-1));

            pos_in_block = (pos_in_block + 1) % block_size;

            if((ele.getLeft() < num_classes) && 
                    (ele.getRight() < num_classes))
                continue;

            else if(ele.getLeft() < num_classes)
                bin_q.push_front(bin[ele.getRight()]);

            else if(ele.getRight() < num_classes)
                bin_q.push_front(bin[ele.getLeft()]); 

            else {
                if(layout.find(std::string("stat")) != std::string::npos ||
                        layout.find(std::string("weighted")) != std::string::npos){

                    if(bin[ele.getLeft()].getCardinality() < 
                            bin[ele.getRight()].getCardinality()){
                        bin_q.push_front(bin[ele.getLeft()]); 
                        bin_q.push_front(bin[ele.getRight()]);
                    }
                    else{
                        bin_q.push_front(bin[ele.getRight()]);
                        bin_q.push_front(bin[ele.getLeft()]); 
                    }
                }
                else{
                    bin_q.push_front(bin[ele.getLeft()]); 
                    bin_q.push_front(bin[ele.getRight()]);
                }
            }
        }

        // set new IDs
        int siz = finalbin.size();
        for (auto i=num_classes; i<siz; i++){
            if(finalbin[i].getLeft() >= num_classes)
                finalbin[i].setLeft(node_to_index[bin[finalbin[i].getLeft()].getID()]);
            if(finalbin[i].getRight() >= num_classes)
                finalbin[i].setRight(node_to_index[bin[finalbin[i].getRight()].getID()]);
        }

        int node_count = 0;
        for (auto node: finalbin){
            node_to_index.insert(std::pair<int, int>(node.getID(), node_count));
            node_count++;
        }
        std::sort(finalbin.begin() + num_classes, finalbin.end(), [this](auto l, auto r){return myCompFunction(l, r);} );

        //replace bin with final bin
    }

    inline std::deque<StatNode<T, F>>  packSubtreeBlockwiseHelper(
            std::vector<StatNode<T, F>>&bin, 
            const int num_trees_in_bin, 
            std::vector<int> &bin_start, 
            std::deque<StatNode<T, F>> &bin_q) { 

        int num_classes = std::atoi(Config::getValue("numclasses").c_str());
        int block_size = std::atoi(Config::getValue("blocksize").c_str());
        int pos_in_block = (finalbin.size()-1) % block_size;

        while(!bin_q.empty()){
            auto ele = bin_q.front();
            if(pos_in_block != block_size - 1){
                bin_q.pop_front();
            }
            else{
                ele = popMaxCardEle(bin_q);
            }

            finalbin.push_back(ele);
            node_to_index.insert(std::pair<int, int>(ele.getID(), 
                        finalbin.size()-1));

            pos_in_block = (pos_in_block + 1) % block_size;

            if((ele.getLeft() < num_classes) && 
                    (ele.getRight() < num_classes))
                continue;

            else if(ele.getLeft() < num_classes)
                bin_q.push_front(bin[ele.getRight()]);

            else if(ele.getRight() < num_classes)
                bin_q.push_front(bin[ele.getLeft()]); 

            else {
                if(layout.find(std::string("stat")) != std::string::npos ||
                        layout.find(std::string("weighted")) != std::string::npos){

                    if(bin[ele.getLeft()].getCardinality() < 
                            bin[ele.getRight()].getCardinality()){
                        bin_q.push_front(bin[ele.getLeft()]); 
                        bin_q.push_front(bin[ele.getRight()]);
                    }
                    else{
                        bin_q.push_front(bin[ele.getRight()]);
                        bin_q.push_front(bin[ele.getLeft()]); 
                    }
                }
                else{
                    bin_q.push_front(bin[ele.getLeft()]); 
                    bin_q.push_front(bin[ele.getRight()]);
                }
            }
        }
    }		


    public:

    Packer(): layout(std::string("bfs")) {depth_intertwined = 0;};
    Packer(std::string layout_str): layout(layout_str){depth_intertwined = 0;}
    Packer(int depth, std::string layout_str): 
        depth_intertwined(depth), layout(layout_str){}

    inline void setDepthIntertwined(const int depth){
        depth_intertwined = depth;
    }

    inline void pack(std::vector<StatNode<T, F>>&bin, 
            const int num_trees_in_bin, std::vector<int> &bin_start){

        const std::string bin_string = "bin";
        int num_classes = std::atoi(Config::getValue("numclasses").c_str());
        for(int i=0; i<num_classes; ++i){
            finalbin.push_back(bin[i]);
        }
        if(layout.find(bin_string) != std::string::npos){
            PackLayoutWithBin(bin, num_trees_in_bin, bin_start);
        }
        else{
            PackRegularLayout(bin, num_trees_in_bin, bin_start);
        }
    }

    inline void PackLayoutWithBin(std::vector<StatNode<T, F>> &bin,
            const int num_trees_in_bin, std::vector<int> &bin_start){

        int num_classes = std::atoi(Config::getValue("numclasses").c_str());

        //Interleaved BIN
        std::deque<StatNode<T, F>> bin_q =
            packBinHelper(bin, num_trees_in_bin, bin_start);

        // STAT per (sub)tree layout 
        if(layout.find(std::string("bfs")) != std::string::npos){
            packSubtreeBFSHelper(bin, num_trees_in_bin, bin_start, bin_q);
        }
        else if(layout.find(std::string("dfs")) != std::string::npos){
            packSubtreeDFSHelper(bin, num_trees_in_bin, bin_start, bin_q);
        }
        else if(layout.find(std::string("class")) != std::string::npos){
            packSubtreeBlockwiseClassHelper(bin, num_trees_in_bin, bin_start, bin_q);
        }
        else{
            packSubtreeBlockwiseHelper(bin, num_trees_in_bin, bin_start, bin_q);
        }

        // set new IDs
        int siz = finalbin.size();
        for (auto i=num_classes; i<siz; i++){
            if(finalbin[i].getLeft() >= num_classes)
                finalbin[i].setLeft(node_to_index[bin[finalbin[i].getLeft()].getID()]);
            if(finalbin[i].getRight() >= num_classes)
                finalbin[i].setRight(node_to_index[bin[finalbin[i].getRight()].getID()]);
        }

        //replace bin with final bin
        std::copy(finalbin.begin(), finalbin.end(), bin.begin());
    }

    inline void PackRegularLayout(std::vector<StatNode<T, F>> &bin,
            const int num_trees_in_bin, std::vector<int> &bin_start){

        std::deque<StatNode<T, F>> bin_q;
        int num_classes = std::atoi(Config::getValue("numclasses").c_str());

        //initialize queue with root node of each tree
        for(int i=0; i<num_trees_in_bin; ++i){
            bin_q.push_back(bin[bin_start[i]]);
        }

        // STAT per (sub)tree layout 
        if(layout.find(std::string("bfs")) != std::string::npos){
            packSubtreeBFSHelper(bin, num_trees_in_bin, bin_start, bin_q);
        }
        else if(layout.find(std::string("dfs")) != std::string::npos){
            packSubtreeDFSHelper(bin, num_trees_in_bin, bin_start, bin_q);
        }
        else if(layout.find(std::string("class")) != std::string::npos){
            packSubtreeBlockwiseClassHelper(bin, num_trees_in_bin, bin_start, bin_q);
        }
        else{
            packSubtreeBlockwiseHelper(bin, num_trees_in_bin, bin_start, bin_q);
        }

        // set new IDs
        int siz = finalbin.size();
        for (auto i=num_classes; i<siz; i++){
            if(finalbin[i].getLeft() >= num_classes)
                finalbin[i].setLeft(node_to_index[bin[finalbin[i].getLeft()].getID()]);
            if(finalbin[i].getRight() >= num_classes)
                finalbin[i].setRight(node_to_index[bin[finalbin[i].getRight()].getID()]);
        }

        //replace bin with final bin
        std::copy(finalbin.begin(), finalbin.end(), bin.begin());
    }
};
#endif
