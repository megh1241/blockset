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
        Node(int l, int r, F feat, T thresh): left(l), right(r), feature(feat), 
        threshold(thresh){};

        inline bool isInternalNode() {
            return left;
        }
        
        inline bool isInternalNodeFront() {
            std::cout<<"Left: "<<left<<"\n";
            return left > -1;
        }

        inline int getLeft() {
            return left;
        }

        inline int setLeft(int ele) {
            left = ele;
        }

        inline int getRight() {
            return right;
        }

        inline void setRight(int ele) {
            right = ele;
        }

        inline void setFeature(F ele) {
            feature = ele;
        }

        inline F getFeature(){
            return feature;
        }


        inline int getClass() {
            return right;
        }

        inline int setClass(int ele) {
            right = ele;
        }

        inline bool goLeft(T feature_val){
            return feature_val <= threshold;
        }

        inline int nextNode(T feature_val) {
            return (feature_val <= threshold) ? left : right;
        }

        inline void virtual printNode() {
            if(isInternalNode())
                std::cout<<"Internal Node ";
            else
                std::cout<<"Leaf Node ";
            std::cout<<"feature: "<<feature<<", threshold: "
                <<threshold<<", left: "<<left<<" ,right: "<<right<<"\n"; 
        }
};

#endif
