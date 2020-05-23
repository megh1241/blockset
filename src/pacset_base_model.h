#ifndef PACSET_BASE
#define PACSET_BASE

#include <vector>
#include <iostream>
#include "stat_node.h"
#include "node.h"

template <typename T, typename F>
class PacsetBaseModel{
    //TODO: (Note) Upcaset to Node when serializing to file
    protected:
        std::vector<std::vector<StatNode<T, F>>> bins;
        std::vector<int> bin_sizes;
        std::vector<int> bin_node_sizes;
        std::vector<std::vector<int>> bin_start;
    public:
        virtual void setMembers(const std::vector<int> &bin_sizes, 
                const std::vector<int> &bin_node_sizes, 
                const std::vector<std::vector<int>> &bin_start) = 0;
        virtual void pack() = 0;
        virtual void loadModel() = 0;
        virtual int predict(const std::vector<T> &observations, 
                std::vector<int> &preds) = 0;
        virtual void predict(const std::vector<std::vector<T>> &observations, 
                std::vector<int> &preds, std::vector<int> &result, bool mmap) = 0;
        virtual void predict(const std::vector<std::vector<T>> &observations, 
                std::vector<int> &preds, std::vector<double> &result, bool mmap) = 0;
        virtual void serialize() = 0;
        virtual void deserialize() = 0;
};

#endif
