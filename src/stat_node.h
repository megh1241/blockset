#ifndef STATNODE_H
#define STATNODE_H

#include <stdio.h>
#include <iostream>
#include "node.h"

template <typename T, typename F>
class StatNode: public Node<T, F>
{
    int cardinality;
    int depth;
    int tree;
    int left_child_cardinality;
    int right_child_cardinality;

    public:
    StatNode(int l, int r, F feat, T thresh, int card): Node<T, F>(l, r, feat, thresh), cardinality(card) {}
    
    StatNode(int l, int r, F feat, T thresh, int card, int d): Node<T, F>(l, r, feat, thresh), cardinality(card), depth(d) {}
    
    StatNode(int l, int r, F feat, T thresh, int card, int d, int t): Node<T, F>(l, r, feat, thresh), 
    cardinality(card), depth(d), tree(t) {}

    StatNode(int l, int r, F feat, T thresh, int card, int d, int t, int lc, int rc): 
        Node<T, F>(l, r, feat, thresh), cardinality(card), depth(d), tree(t), 
        left_child_cardinality(lc), right_child_cardinality(rc) {}

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
};

#endif
