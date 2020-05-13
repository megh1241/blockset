#ifndef PACSET_RF_CLASS
#define PACSET_RF_CLASS

#include <vector>
#include <unordered_set>
#include <fstream>

#include "pacset_base_model.h"
#include "packer.h"
#include "config.h"
#include "json_reader.h"
#include "utils.h"
#include "node.h"
#include "MemoryMapped.h"

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
            int num_bins = PacsetBaseModel<T, F>::bins.size();

            std::unordered_set<int> blocks_accessed;
            int block_offset = 0;
            int block_number = 0;
#pragma omp parallel for num_threads(num_threads)
            for(int bin_counter=0; bin_counter<num_bins; ++bin_counter){
                auto bin = PacsetBaseModel<T, F>::bins[bin_counter];

                std::vector<int> curr_node(PacsetBaseModel<T, F>::bin_node_sizes[bin_counter]);
                int feature_num=0, number_not_in_leaf=0;
                T feature_val;
                int siz = PacsetBaseModel<T, F>::bin_sizes[bin_counter];

                for(i=0; i<siz; ++i){
                    curr_node[i] = PacsetBaseModel<T, F>::bin_start[bin_counter][i];
                    __builtin_prefetch(&bin[curr_node[i]], 0, 3);
#ifdef BLOCK_LOGGING 
                    block_number = (curr_node[i] + block_offset) / BLOCK_SIZE;
#pragma omp critical
                    blocks_accessed.insert(block_number);
#endif
                }

                do{
                    number_not_in_leaf = 0;
                    for( i=0; i<siz; ++i){
                        if(bin[curr_node[i]].isInternalNodeFront()){
#ifdef BLOCK_LOGGING 
                            block_number = (curr_node[i] + block_offset)/ BLOCK_SIZE;
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

            //Predicts the class for a vector of observations
            //By calling predict for a single observation and 
            //tallying the observations
            //

            int num_classes = std::stoi(Config::getValue("numclasses"));
            int num_bins; 
            std::string layout = Config::getValue("layout");
            std::string num_threads = Config::getValue("numthreads");
            std::string dataset = Config::getValue("datafilename");
            std::string intertwine = Config::getValue("intertwine");
            
            for(int i=0; i<num_classes; ++i){
                preds.push_back(0);
            }

            int max = -1;
            int maxid = -1;
            int blocks;
            std::vector<int> num_blocks;
            for(auto single_obs : observation){
                blocks = predict(single_obs, preds);
                num_blocks.push_back(blocks);
                //TODO: change
                for(int i=0; i<num_classes; ++i){
                    if(preds[i]>max){
                        maxid = i;
                        max = preds[i];
                    }
                }
                int count = std::count(std::begin(preds), std::end(preds), max);
                results.push_back(maxid); 
                std::fill(preds.begin(), preds.end(), 0);
            }

#ifdef BLOCK_LOGGING 
            std::fstream fout;
            std::string filename = "logs/Blocks_" + 
                layout + "threads_" + num_threads +
                + "intertwine_"  + intertwine + ".csv";
            fout.open(filename, std::ios::out | std::ios::app);
            for(auto i: num_blocks){
                fout<<i<<",";
            }
            fout.close();
#endif
        }

        inline void deserialize() {
        }

        inline void serialize() {
            auto bins = PacsetBaseModel<T, F>::bins;
            int num_classes = std::stoi(Config::getValue("numclasses"));
            int num_bins = bins.size();
            std::vector<int> bin_sizes = PacsetBaseModel<T, F>::bin_sizes;
            std::vector<int> bin_node_sizes = PacsetBaseModel<T, F>::bin_node_sizes;
            std::vector<std::vector<int>> bin_start  = PacsetBaseModel<T, F>::bin_start;
            std::string format = Config::getValue("format");
            
            //Write the metadata needed to reconstruct bins and for prediction
            //TODO: change filename
            std::string filename = "metadata.txt";
            std::fstream fout;
            fout.open(filename, std::ios::out );

            //Number of classes
            fout<<num_classes<<"\n";

            //Number of bins
            fout<<num_bins<<"\n";

            //Number of trees in each bin
            for(auto i: bin_sizes){
                fout<<i<<"\n";
            }

            //Number of nodes in each bin
            for(auto i: bin_node_sizes){
                fout<<i<<"\n";
            }

            //start position of each bin
            for(auto bin: bin_start){
                for(auto tree_start: bin){
                    fout<<tree_start<<"\n";
                }
            }
            fout.close();
            
            if(format != std::string("notfound") ||
                    format == std::string("binary")){

                std::string modelfname = Config::getValue("modelfilename2");
                std::string filename;

                if(modelfname != std::string("notfound"))
                    filename = modelfname;
                else
                    filename = "packedmodel.bin";

                //Write the nodes
                fout.open(filename, std::ios::binary | std::ios::out );
                Node<T, F>* node_to_write;
                for(auto bin: bins){
                    for(auto node: bin){
                        node_to_write = &node;
                        fout.write((char*)&node_to_write, sizeof(node_to_write));
                    }
                }
                fout.close();

            }
            else{
                //Write the nodes
                std::string modelfname = Config::getValue("packfilename");
                std::string filename;

                std::cout<<"modelfname: "<<modelfname<<"\n";

                filename = "packedmodel.txt";

                std::cout<<"filename: "<<filename <<"\n";
                fout.open(filename,  std::ios::out );
                for(auto bin: bins){
                    for(auto node: bin){
                        fout<<node.getLeft()<<", "<<node.getRight()<<", "<<node.getFeature()<<", "<<node.getThreshold()<<"\n";
                    }
                }
                fout.close();
            }
        }

        inline void readMetadata(){
            //Write the metadata needed to reconstruct bins and for prediction
            //TODO: change filename
            int num_classes, num_bins;
            std::string filename = "metadata.txt";
            std::fstream f;
            f.open(filename, std::ios::in );

            //Number of classes
            f>>num_classes;
            Config::setValue("numclasses", std::to_string(num_classes));
            //Number of bins
            f>>num_bins;
            Config::setValue("numthreads", std::to_string(num_bins))

            //Number of trees in each bin
            for(int=i=0; i<num_bins; ++i){
                f>>;
            }

            //Number of nodes in each bin
            for(int=i=0; i<num_bins; ++i){
                f>>i;
            }

            //start position of each bin
            for(auto bin: bin_start){
                for(auto tree_start: bin){
                    fout<<tree_start<<"\n";
                }
            }
            fout.close();
            
        }

        inline void deserialize() {
            readMetadata();
            std::string modelfname = Config::getValue("modelfilename");
            MemoryMapped mmapped_obj(modelfname, 0);
            Node<T, F> *data = (Node<T, F>*)mmapped_obj.getData();
        }
};

#endif
