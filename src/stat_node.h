#ifndef STATNODE_H
#define STATNODE_H

#include <stdio.h>
#include <iostream>
#include "node.h"

template <typename T, typename F>
class StatNode: public Node<T, F>
{
    int id;
    int cardinality;
    int depth;
    int tree;
    int left_child_cardinality;
    int right_child_cardinality;
    int block_number;
    int subtree_number;

    public:
    StatNode(){}
    StatNode(int l, int r, F feat, T thresh, int card): 
        Node<T, F>(l, r, feat, thresh), cardinality(card) {}
    
    StatNode(int l, int r, F feat, T thresh, int card, 
            int selfid, int d): 
        Node<T, F>(l, r, feat, thresh),
        cardinality(card), id(selfid),
        depth(d){}


    inline int getID(){
        return id;
    }

    inline void setID(int new_id){
        id = new_id;
    }

    inline int getCardinality(){
        return cardinality;
    }

    inline void setCardinality(int c){
        cardinality = c;
    }

    inline int getDepth(){
        return depth;
    }

    inline void setDepth(int d){
        depth = d;
    }

    inline int getTreeID(){
        return tree;
    }

    inline void setTreeID(int id){
        tree = id;
    }

    inline void setBlockNum(int num){
        block_number = num;
    }

    inline int getBlockNum(){
        return block_number;
    }
    
    inline void setSubtreeNum(int num){
        subtree_number = num;
    }

    inline int getSTNum(){
        return subtree_number;
    }

    inline int setSTNum(int num){
        subtree_number = num;
    }

    inline int getLeftCardinality(){
        return left_child_cardinality;
    }

    inline void setLeftCardinality(int lc){
        left_child_cardinality = lc;
    }

    inline int getRightCardinality(){
        return right_child_cardinality;
    }

    inline void setRightCardinality(int lc){
        right_child_cardinality = lc;
    }

    inline void printNode(){
         std::cout<<"id: "<<id<<"feature: "<<Node<T, F>::feature
             <<" ,threshold: "<<Node<T, F>::threshold<<", left: "
             <<Node<T, F>::left<<" ,right: "<<Node<T, F>::right
             <<", cardinality: "<<cardinality<<"\n";
    }
};

#endif
