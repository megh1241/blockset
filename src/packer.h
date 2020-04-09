#ifndef PACKER_H
#define PACKER_H

#include <iostream>
#include "stat_node.h"
#include "node.h"
#include <string>
#include <vector>

template <typename T, typename F>
class Packer{
   
    std::string layout;
    
    public:
        Packer(): layout(std::string("bfs")) {};
        Packer(std::string layout_str): layout(layout_str){}
        
        inline void pack(std::vector<std::vector<StatNode<T, F>>>&bins){
             if(layout == "bfs")
                 BFSLayout(bins);
             else if(layout == "dfs")
                 DFSLayout(bins);
             else if(layout == "wdfs")
                 weightedDFSLayout(bins);
             else if(layout == "binbfs")
                 BINBFSLayout(bins);
             else if(layout == "bindfs")
                 BINDFSLayout(bins);
             else
                 BINWDFSLayout(bins);
        }

        inline void BFSLayout(std::vector<std::vector<StatNode<T, F>>>&bins){
        }
        
        inline void DFSLayout(std::vector<std::vector<StatNode<T, F>>>&bins){
        }
        
        inline void weightedDFSLayout(std::vector<std::vector<StatNode<T, F>>> &bins){
        }
        
        inline void BINBFSLayout(std::vector<std::vector<StatNode<T, F>>> &bins){
        }
        
        inline void BINDFSLayout(std::vector<std::vector<StatNode<T, F>>> &bins){
        }
        
        inline void BINWDFSLayout(std::vector<std::vector<StatNode<T, F>>> &bins){
        }

        inline void BINBlockLayout(std::vector<std::vector<StatNode<T, F>>> &bins){
        }
};
#endif
