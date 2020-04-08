#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <iostream>

template <typename T, typename F>
class alignas(32) Node
{
    protected:
        int left;
        int right;
        F feature;
        T threshold;
    public:
        Node(): left(0), right(0){}
        Node(int l, int r, F feat, T thresh): left(l), right(r), feature(feat), threshold(thresh){};

        bool isInternalNode();
        int getLeft();
        int setLeft(int ele);
        int getRight();
        void setRight(int ele);
        int getClass();
        int setClass(int ele);
        int nextNode(T feature_val);
        inline void virtual printNode() {
            if(isInternalNode())
                std::cout<<"Internal Node ";
            else
                std::cout<<"Leaf Node ";
            std::cout<<"Threshold: "<<threshold<<", left: "<<left<<" ,right: "<<right<<"\n"; 
        }
};

#endif
