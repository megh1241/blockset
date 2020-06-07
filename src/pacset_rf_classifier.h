#ifndef PACSET_RF_CLASS
#define PACSET_RF_CLASS

#include <vector>
#include <unordered_set>
#include <fstream>
#include <chrono>
#include <random>
#include "pacset_base_model.h"
#include "packer.h"
#include "config.h"
#include "json_reader.h"
#include "utils.h"
#include "node.h"
#include "MemoryMapped.h"

#define BLOCK_LOGGING 1
#define NUM_FILES 999
#define BLOCK_SIZE 128

template <typename T, typename F>
class PacsetRandomForestClassifier: public PacsetBaseModel<T, F> {
    public:

        inline void setMembers(const std::vector<int> &bin_sizes,
                const std::vector<int> &bin_node_sizes,
                const std::vector<std::vector<int>> &bin_start){
            
            std::copy(bin_sizes.begin(), bin_sizes.end(), back_inserter(PacsetBaseModel<T, F>::bin_sizes)); 
            std::copy(bin_node_sizes.begin(), bin_node_sizes.end(), back_inserter(PacsetBaseModel<T, F>::bin_node_sizes)); 
            for (auto i: bin_start)
                PacsetBaseModel<T, F>::bin_start.push_back(i);  
        }


        inline void loadModel() {
            JSONReader<T, F> J;
            //J.convertSklToBins(PacsetBaseModel<T, F>::bins, 
            J.convertSklToBinsRapidJson(PacsetBaseModel<T, F>::bins, 
                    PacsetBaseModel<T, F>::bin_sizes, 
                    PacsetBaseModel<T, F>::bin_start,
                    PacsetBaseModel<T, F>::bin_node_sizes);
        	std::cout<<"converted\n";
		fflush(stdout);
        std::cout<<"test data loaded\n";
	}

        inline void pack(){
		std::cout<<"ppoint 1\n";
	    fflush(stdout);	
	std::string layout = Config::getValue("layout");
		std::cout<<"ppoint 2\n";
	    fflush(stdout);	
		
            auto bin = PacsetBaseModel<T, F>::bins[0];
		std::cout<<"ppoint 3\n";
	    fflush(stdout);	
            int num_bins = std::stoi(Config::getValue("numthreads"));
		std::cout<<"ppoint 3\n";
	    fflush(stdout);	
            for(int i=0; i<num_bins; ++i){
		    std::cout<<"bin number: "<<i<<"\n";
		    fflush(stdout);
		   Packer<T, F> packer_obj(layout);
		std::cout<<"initiallize packer obj\n";
		fflush(stdout);
                if(Config::getValue("intertwine") != std::string("notfound"))
                    packer_obj.setDepthIntertwined(std::atoi(Config::getValue("intertwine").c_str()));
		std::cout<<"before pack\n";
		fflush(stdout);
		for (auto i: PacsetBaseModel<T, F>::bin_start){
			for(auto j: i)
				std::cout<<j<<"\n";
		}
                //should pack in place
                packer_obj.pack(PacsetBaseModel<T, F>::bins[i], 
                        PacsetBaseModel<T, F>::bin_sizes[i],
                        PacsetBaseModel<T, F>::bin_start[i] 
                        );
		std::cout<<"after pack\n";
		fflush(stdout);
		for (auto i: PacsetBaseModel<T, F>::bin_start){
			for(auto j: i)
				std::cout<<j<<"\n";
		}
            }
        }

        inline int predict(const std::vector<T>& observation, std::vector<double>& preds) {}
        
            
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
                int i, feature_num=0, number_not_in_leaf=0;
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
        
        void predict(const std::vector<std::vector<T>> &observations,
                        std::vector<double> &preds, std::vector<double> &result, bool mmap) {}

        inline int mmapAndPredict(const std::vector<T>& observation, std::vector<int>& preds, int obsnum) {
            int num_classes = std::stoi(Config::getValue("numclasses"));
            int num_threads = std::stoi(Config::getValue("numthreads"));
            int num_bins = PacsetBaseModel<T, F>::bin_sizes.size();
            std::string modelfname = Config::getValue("modelfilename");
            
#ifdef LAT_LOGGING 
	    MemoryMapped mmapped_obj((modelfname + std::to_string(obsnum % NUM_FILES) + ".bin").c_str(), 0);
#else
	    MemoryMapped mmapped_obj(modelfname.c_str(), 0);
#endif
	    Node<T, F> *data = (Node<T, F>*)mmapped_obj.getData();
            
            std::unordered_set<int> blocks_accessed;
            int block_offset = 0;
            int offset = 0;
            std::vector<int> offsets;
            int curr_offset = 0;
            for (auto val: PacsetBaseModel<T, F>::bin_node_sizes){
                offsets.push_back(curr_offset);
                curr_offset += val;
            }
#pragma omp parallel for num_threads(num_threads)
            for(int bin_counter=0; bin_counter<num_bins; ++bin_counter){
                int block_number = 0;
                Node<T, F> *bin  = data + offsets[bin_counter];

                std::vector<int> curr_node(PacsetBaseModel<T, F>::bin_node_sizes[bin_counter]);
                int i, feature_num=0, number_not_in_leaf=0;
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
                block_offset += PacsetBaseModel<T, F>::bin_node_sizes[bin_counter];
            }
        mmapped_obj.close();
#ifdef BLOCK_LOGGING 
            return blocks_accessed.size();
#else
            return 0;
#endif
	}


        inline void predict(const std::vector<std::vector<T>>& observation, 
                std::vector<int>& preds, std::vector<int>&results, bool mmap) {

            //Predicts the class for a vector of observations
            //By calling predict for a single observation and 
            //tallying the observations
            //

            int num_classes = std::stoi(Config::getValue("numclasses"));
            int num_bins; 
	    std::vector<double> elapsed_arr;
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
	    int ct=0;
            std::vector<int> num_blocks;
            for(auto single_obs : observation){
		    std::cout<<"start obs\n";
		auto start = std::chrono::steady_clock::now();
	       	if (mmap)
                    blocks = mmapAndPredict(single_obs, preds, ct+1);
                else
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
		auto end = std::chrono::steady_clock::now();
#ifdef LAT_LOGGING
		double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
		elapsed_arr.push_back(elapsed);
#endif
		std::cout<<"observation done: "<<ct<<"\n";
		ct++;
                results.push_back(maxid); 
                std::fill(preds.begin(), preds.end(), 0);
                max = -1;
                maxid = -1;
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
#ifdef LAT_LOGGING
            std::fstream fout2;
            std::string filename2 = "logs/latency_" + 
                layout + "threads_" + num_threads +
                + "intertwine_"  + intertwine + ".csv";
            fout2.open(filename2, std::ios::out | std::ios::app);
            for(auto i: elapsed_arr){
                fout2<<i<<",";
            }
            fout2.close();
#endif
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
            for(auto i: bin_start){
                for(auto tree_start: i){
			std::cout<<tree_start<<"\n";
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
                Node<T, F> node_to_write;
                for(auto bin: bins){
			for(auto node: bin){
                        node_to_write = node;
			std::cout<<"Size of node to write: "<< sizeof(node_to_write)<<"\n";
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
                        fout<<node.getLeft()<<", "<<node.getRight()
                            <<", "<<node.getFeature()<<", "<<node.getThreshold()<<"\n";
                    }
                }
                fout.close();
            }
        }

        inline void deserialize(){
            //Write the metadata needed to reconstruct bins and for prediction
            //TODO: change filename
            int num_classes, num_bins;
	    std::string filename = Config::getValue("metadatafilename");
	    //std::string filename = "metadata.txt";
            std::fstream f;
            f.open(filename, std::ios::in );

            //Number of classes
            f>>num_classes;
            Config::setConfigItem("numclasses", std::to_string(num_classes));
            //Number of bins
            f>>num_bins;
            Config::setConfigItem("numthreads", std::to_string(num_bins));
            std::vector<int> num_trees_bin;
            std::vector<int> num_nodes_bin;
            std::vector<std::vector<int>> bin_tree_start;
            int val;
            //Number of trees in each bin
            for(int i=0; i<num_bins; ++i){
                f>>val;
                num_trees_bin.push_back(val);
            }

            //Number of nodes in each bin
            for(int i=0; i<num_bins; ++i){
                f>>val;
                num_nodes_bin.push_back(val);
            }

            std::vector<int> temp;
            //start position of each bin
            for(int i=0; i<num_bins; ++i){
                for(int j=0; j<num_trees_bin[i]; ++j){
                    f>>val;
                    temp.push_back(val); 
                }
                bin_tree_start.push_back(temp);
                temp.clear();
            }
            f.close();
            setMembers(num_trees_bin, num_nodes_bin, bin_tree_start);

        }

        /*
         inline void deserialize() {
            readMetadata();
            std::string modelfname = Config::getValue("modelfilename");
            MemoryMapped mmapped_obj(modelfname, 0);
            Node<T, F> *data = (Node<T, F>*)mmapped_obj.getData();
            //TODO: make this a separate predict bin
            std::vector<std::vector<Node<T, F>>> bins;   
            int pos = 0;
            for (auto i: PacsetBaseModel<T, F>::bin_node_sizes){
                std::vector<StatNode<T, F>> nodes;
                nodes.assign(data+pos, data+pos+i);
                bins.push_back(nodes);
                pos = i;
            }
        
        }
        */
};

#endif
