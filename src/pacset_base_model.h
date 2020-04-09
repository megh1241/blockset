#ifndef PACSET_BASE
#define PACSET_BASE

#include <vector>
#include <iostream>
#include "stat_node.h"
#include "node.h"

class PacsetBaseModel{
    //TODO: (Note) Upcaset to Node when serializing to file
    std::vector<std::vector<StatNode<float, float>>> bins;
    public:
        virtual void pack() = 0;
        virtual int predict() = 0;
        virtual void serialize() = 0;
        virtual void deserialize() = 0;
};


#endif
