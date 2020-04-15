#ifndef PACSET_RF_CLASS
#define PACSET_RF_CLASS

#include <vector>
#include "pacset_base_model.h"
#include "packer.h"
#include "config.h"
#include "json_reader.h"
#include "utils.h"

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
            Packer<T, F> packer_obj(layout);
            packer_obj.pack(PacsetBaseModel<T, F>::bins);
        }

        inline void predict(const std::vector<T>& observation, std::vector<int>& preds) {
            int bin_counter = 0;
            int num_classes = std::stoi(Config::getValue("numclasses"));

            for(auto bin: PacsetBaseModel<T, F>::bins){
                std::vector<int> curr_node(PacsetBaseModel<T, F>::bin_node_sizes[bin_counter]);
                int i=0, feature_num=0, number_not_in_leaf=0;
                T feature_val;
                int siz = PacsetBaseModel<T, F>::bin_sizes[bin_counter];
                for(i=0; i<siz; ++i){
                    curr_node[i] = PacsetBaseModel<T, F>::bin_start[bin_counter][i];
                    __builtin_prefetch(&bin[curr_node[i]], 0, 3);
                }

                do{
                    number_not_in_leaf = 0;
                    for( i=0; i<siz; ++i){
                        if(bin[curr_node[i]].isInternalNodeFront()){
                            feature_num = bin[curr_node[i]].getFeature();
                            feature_val = observation[feature_num];
                            curr_node[i] = bin[curr_node[i]].nextNode(feature_val);
                            __builtin_prefetch(&bin[curr_node[i]], 0, 3);
                            ++number_not_in_leaf;
                        }
                    }
                }while(number_not_in_leaf);

                for(i=0; i<siz; ++i){
                    ++preds[bin[curr_node[i]].getClass()];
                }
                ++bin_counter;
            }
        }

        inline void predict(const std::vector<std::vector<T>>& observation, 
                std::vector<int>& preds, std::vector<int>&results) {
            int num_classes = std::stoi(Config::getValue("numclasses"));
            for(int i=0; i<num_classes; ++i){
                preds.push_back(0);
            }

            int predict_value;
            for(auto single_obs : observation){
                predict(single_obs, preds);
                predict_value = static_cast<int>(std::distance(preds.begin(), 
                            max_element(preds.begin(), preds.end())));
                results.push_back(predict_value); 
                std::fill(preds.begin(), preds.end(), 0);
            }
        }

        inline void serialize() {
        }

        inline void deserialize() {
        }
};

#endif
