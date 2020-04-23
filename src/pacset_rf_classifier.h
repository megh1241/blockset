#ifndef PACSET_RF_CLASS
#define PACSET_RF_CLASS

#include <vector>
#include <unordered_set>

#include "pacset_base_model.h"
#include "packer.h"
#include "config.h"
#include "json_reader.h"
#include "utils.h"
#define BLOCK_LOGGING 1
#define BLOCK_SIZE 128

template <typename T, typename F>
class PacsetRandomForestClassifier: public PacsetBaseModel<T, F> {
    public:
        inline void loadModel() {
            JSONReader<T, F> J;
            J.convertToBins(PacsetBaseModel<T, F>::bins, 
                    PacsetBaseModel<T, F>::bin_sizes, 
                    PacsetBaseModel<T, F>::bin_start,
                    PacsetBaseModel<T, F>::bin_node_sizes);
        }

        inline void pack(){
            std::string layout = Config::getValue("layout");
            
            auto bin = PacsetBaseModel<T, F>::bins[0];
            int num_bins = std::stoi(Config::getValue("numthreads"));
            for(int i=0; i<num_bins; ++i){
                Packer<T, F> packer_obj(layout);

                if(Config::getValue("intertwine") != std::string("notfound"))
                    packer_obj.setDepthIntertwined(std::atoi(Config::getValue("intertwine").c_str()));
                //should pack in place
                packer_obj.pack(PacsetBaseModel<T, F>::bins[i], 
                        PacsetBaseModel<T, F>::bin_sizes[i],
                        PacsetBaseModel<T, F>::bin_start[i] 
                );
            }
        }

        inline int predict(const std::vector<T>& observation, std::vector<int>& preds) {
            int num_classes = std::stoi(Config::getValue("numclasses"));
            int num_threads = std::stoi(Config::getValue("numthreads"));
            std::unordered_set<int> blocks_accessed;
            int block_offset = 0;
#pragma omp parallel for num_threads(num_threads)
            for(int bin_counter=0; bin_counter<num_threads; ++bin_counter){
                auto bin = PacsetBaseModel<T, F>::bins[bin_counter];
                
            
                std::vector<int> curr_node(PacsetBaseModel<T, F>::bin_node_sizes[bin_counter]);
                int i=0, feature_num=0, number_not_in_leaf=0;
                T feature_val;
                int siz = PacsetBaseModel<T, F>::bin_sizes[bin_counter];
               
                for(i=0; i<siz; ++i){
                    curr_node[i] = PacsetBaseModel<T, F>::bin_start[bin_counter][i];
                    __builtin_prefetch(&bin[curr_node[i]], 0, 3);
#ifdef BLOCK_LOGGING 
                    int block_number = (curr_node[i] + block_offset) / BLOCK_SIZE;
#pragma omp critical
                    blocks_accessed.insert(block_number);
#endif
                }

                do{
                    number_not_in_leaf = 0;
                    for( i=0; i<siz; ++i){
                        if(bin[curr_node[i]].isInternalNodeFront()){
#ifdef BLOCK_LOGGING 
                    int block_number = (curr_node[i] + block_offset)/ BLOCK_SIZE;
    #pragma omp critical
                    blocks_accessed.insert(block_number);
#endif
                            feature_num = bin[curr_node[i]].getFeature();
                            feature_val = observation[feature_num];
                            curr_node[i] = bin[curr_node[i]].nextNode(feature_val);
                            __builtin_prefetch(&bin[curr_node[i]], 0, 3);
                            ++number_not_in_leaf;
                        }
                    }
                }while(number_not_in_leaf);

                for(i=0; i<siz; ++i){

#pragma omp atomic update
                    ++preds[bin[curr_node[i]].getClass()];
                }

#pragma omp critical
                block_offset += bin.size();
            }
#ifdef BLOCK_LOGGING 
            return blocks_accessed.size();
#else
            return 0;
#endif
        }

        inline void predict(const std::vector<std::vector<T>>& observation, 
                std::vector<int>& preds, std::vector<int>&results) {
            int num_classes = std::stoi(Config::getValue("numclasses"));
            for(int i=0; i<num_classes; ++i){
                preds.push_back(0);
            }
            int times_max = 0;
            int obs_num = 0;
            int predict_value;
            int max = -1;
            int maxid = -1;
            for(auto single_obs : observation){
                int blocks = predict(single_obs, preds);
                //TODO: change
                for(int i=0; i<num_classes; ++i){
                    if(preds[i]>max){
                        maxid = i;
                        max = preds[i];
                    }
                }
                int count = std::count(std::begin(preds), std::end(preds), max);
                results.push_back(maxid); 
                maxid = -1;
                max = -1;
                std::fill(preds.begin(), preds.end(), 0);
                //std::cout<<"Obs: "<<obs_num<<"\n";
                obs_num++;
            }
        }

        inline void serialize() {
        }

        inline void deserialize() {
        }
};

#endif
