#ifndef PACSET_GB
#define PACSET_GB

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
#define NUM_FILES 10
#define BLOCK_LOGGING 1
#define LAT_LOGGING 1

template <typename T, typename F>
class PacsetGradientBoostedClassifier: public PacsetBaseModel<T, F> {
    public:

        inline void setMembers(const std::vector<int> &bin_sizes,
                const std::vector<int> &bin_node_sizes,
                const std::vector<std::vector<int>> &bin_start){
            
            std::copy(bin_sizes.begin(), bin_sizes.end(), back_inserter(PacsetBaseModel<T, F>::bin_sizes)); 
            std::copy(bin_node_sizes.begin(), bin_node_sizes.end(), back_inserter(PacsetBaseModel<T, F>::bin_node_sizes)); 
            for (auto i: bin_start)
                PacsetBaseModel<T, F>::bin_start.push_back(i);  
        }

                inline void setBinNodeSizes(int pos, int siz){
                        PacsetBaseModel<T, F>::bin_node_sizes[pos] = siz;
                }
        inline void loadModel() {
            JSONReader<T, F> J;
            //J.convertSklToBins(PacsetBaseModel<T, F>::bins, 
            J.convertXG(PacsetBaseModel<T, F>::bins, 
                    PacsetBaseModel<T, F>::bin_sizes, 
                    PacsetBaseModel<T, F>::bin_start,
                    PacsetBaseModel<T, F>::bin_node_sizes);
        }

        inline void pack(){
            std::string layout = Config::getValue("layout");
	    std::cout<<"layout "<<layout<<"\n";
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
		 setBinNodeSizes(i, PacsetBaseModel<T, F>::bins[i].size());
            }
        }

        inline int predict(const std::vector<T>& observation, std::vector<double> &preds) {
            int num_classes = std::stoi(Config::getValue("numclasses"));
            int num_threads = std::stoi(Config::getValue("numthreads"));
            
	    int num_bins = PacsetBaseModel<T, F>::bins.size();
            int total_num_trees = 0;
            double leaf_sum = 0;
            std::unordered_set<int> blocks_accessed;
	    std::vector<double> elapsed_arr;
            
            int block_offset = 0;
            int block_number = 0;
            int next_node = 0;
#pragma omp parallel for num_threads(num_threads)
            for(int bin_counter=0; bin_counter<num_bins; ++bin_counter){
                auto bin = PacsetBaseModel<T, F>::bins[bin_counter];

                std::vector<int> curr_node(PacsetBaseModel<T, F>::bin_node_sizes[bin_counter]);
                std::vector<double> last_node(PacsetBaseModel<T, F>::bin_node_sizes[bin_counter]);
                int i, feature_num=0, number_not_in_leaf=0;
                T feature_val;
                int siz = PacsetBaseModel<T, F>::bin_sizes[bin_counter];
                total_num_trees = siz;
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
		        if(curr_node[i] >= 0){
#ifdef BLOCK_LOGGING 
                    	    block_number = (curr_node[i] + block_offset)/ BLOCK_SIZE;
#pragma omp critical
                            blocks_accessed.insert(block_number);
#endif
                            feature_num = bin[curr_node[i]].getFeature();
                            feature_val = observation[feature_num];
                            if(bin[curr_node[i]].getLeft() == -1){
                                last_node[i] = bin[curr_node[i]].getThreshold();
                                curr_node[i] = -1;
			    }
			    else {
			        curr_node[i] = bin[curr_node[i]].nextNode(feature_val);
                                __builtin_prefetch(&bin[curr_node[i]], 0, 3);
                                ++number_not_in_leaf;
			    }
                        }
		    }
                }while(number_not_in_leaf);
                
                double sum=0;
                for(i=0; i<siz; ++i){
                    sum += last_node[i];
                }

#pragma omp critical
                {
                leaf_sum +=sum;
                block_offset += bin.size();
                }
            }
            preds.clear();
            preds.push_back(leaf_sum);
	    std::cout<<"leaf sum:: "<<leaf_sum<<"\n";
	    preds.push_back((double)total_num_trees);
#ifdef BLOCK_LOGGING 
            return blocks_accessed.size();
#else
            return 0;
#endif
        }
        

        inline int predict(const std::vector<T>& observation, std::vector<int> &preds) {}
        inline int mmapAndPredict(const std::vector<T>& observation, std::vector<double> &preds, int obsnum) {
            int num_classes = std::stoi(Config::getValue("numclasses"));
            int num_threads = std::stoi(Config::getValue("numthreads"));
            int num_bins = PacsetBaseModel<T, F>::bin_sizes.size();
	    int total_num_trees = 0;
	    
	    std::for_each( PacsetBaseModel<T, F>::bin_sizes.begin(), 
			    PacsetBaseModel<T, F>::bin_sizes.end(), [&] (int n) {
    				total_num_trees += n;
				});

	    std::vector<double> elapsed_arr;
	    int num_boosters = total_num_trees / num_classes;
            std::vector<std::vector<float> > result_mat(
    					num_boosters,
    			std::vector<float>(num_classes)) ;
	   
	    std::vector<float> pred_mat(num_classes); 
	    std::string modelfname = Config::getValue("modelfilename");
            
#ifdef LAT_LOGGING
	    MemoryMapped mmapped_obj(modelfname.c_str(), 0);
	    //MemoryMapped mmapped_obj(("/dat" + std::to_string(obsnum % NUM_FILES) + "/" + modelfname).c_str(), 0);
            //MemoryMapped mmapped_obj((modelfname + std::to_string(obsnum % NUM_FILES) + ".bin").c_str(), 0);
#else
	    MemoryMapped mmapped_obj(modelfname.c_str(), 0);
#endif
	    
	    Node<T, F> *data = (Node<T, F>*)mmapped_obj.getData();
            std::unordered_set<int> blocks_accessed;
            int next_node = 0;
            int block_offset = 0;
            int offset = 0;
            double leaf_sum = 0;
            std::vector<int> offsets;
            std::vector<int> tree_offsets;
            int curr_offset = 0;
	    int bin_tree_offset = 0;
            total_num_trees=0;
	    for (auto val: PacsetBaseModel<T, F>::bin_node_sizes){
                offsets.push_back(curr_offset);
                curr_offset += val;
            }
	    tree_offsets.push_back(0);
	    int it = 0;
	    for(auto val: PacsetBaseModel<T, F>::bin_sizes){
	    	tree_offsets.push_back(tree_offsets[it++] +  val);
	    }
#pragma omp parallel for num_threads(num_threads)
            for(int bin_counter=0; bin_counter<num_bins; ++bin_counter){
                int block_number = 0;
		Node<T, F> *bin  = data + offsets[bin_counter];
                std::vector<int> curr_node(PacsetBaseModel<T, F>::bin_sizes[bin_counter]);
                int i, feature_num=0, number_not_in_leaf=0;
                T feature_val;
                int siz = PacsetBaseModel<T, F>::bin_sizes[bin_counter];
		total_num_trees += siz;
		bin_tree_offset = tree_offsets[bin_counter];
		std::cout<<"printing in predict!\n";
		//for(int i=0; i<PacsetBaseModel<T, F>::bin_node_sizes[0]; ++i)
		//	bin[i].printNode();
                for(i=0; i<siz; ++i){
                    curr_node[i] = PacsetBaseModel<T, F>::bin_start[bin_counter][i];
                    //bin[curr_node[i]].printNode();
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
		
			if(curr_node[i] >= 0){
			    std::cout<<i<<": "<<curr_node[i]<<"\n";
			    bin[curr_node[i]].printNode();
#ifdef BLOCK_LOGGING 
                    	    block_number = (curr_node[i] + block_offset)/ BLOCK_SIZE;
#pragma omp critical
                            blocks_accessed.insert(block_number);
#endif
                            feature_num = bin[curr_node[i]].getFeature();
                            feature_val = observation[feature_num];
                            if(bin[curr_node[i]].getLeft() == -1){
				pred_mat[(bin_tree_offset+i)%num_classes] += bin[curr_node[i]].getThreshold();
				//result_mat[(bin_tree_offset+i)/num_classes][(bin_tree_offset+i)%num_classes] = bin[curr_node[i]].getThreshold();
                                curr_node[i] = -1;
			    }
			    else {
			        curr_node[i] = bin[curr_node[i]].nextNode(feature_val);
                                __builtin_prefetch(&bin[curr_node[i]], 0, 3);
                                ++number_not_in_leaf;
			    }
                        }
		    }
                }while(number_not_in_leaf);

#pragma omp critical
                {
                block_offset += PacsetBaseModel<T, F>::bin_node_sizes[bin_counter];
                }

            }
	    //std::vector<float>result_mat_proba = logit(pred_mat);
	    std::vector<float>result_mat_proba(pred_mat);
	    /*for(auto ele: result_mat_proba){
            	std::cout<<ele<<", ";
	    }*/
	    std::cout<<"\n";
	    int max = -999;
	    int maxid = 0;
	    for(int i=0; i<num_classes; ++i){
            	if(result_mat_proba[i]>max){
                    maxid = i;
                    max = result_mat_proba[i];
                }
            }
	    /*for(auto row: result_mat){
	    	for(auto ele: row){
			std::cout<<ele<<", ";
		}
		std::cout<<"\n";
	    }
	    */
            preds.clear();
            preds.push_back((double)maxid);
            preds.push_back((double)1);
#ifdef BLOCK_LOGGING 
            return blocks_accessed.size();
#else
            return 0;
#endif
        }

        inline void predict(const std::vector<std::vector<T>>& observation, 
                std::vector<int>& preds, std::vector<int>&results,  bool mmap) {

        }

        inline void predict(const std::vector<std::vector<T>>& observation, 
               std::vector<double>& preds, std::vector<double>&results, bool mmap) {

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
            
	    std::vector<double> elapsed_arr;
            int blocks;
            std::vector<int> num_blocks;
	    int ct=1;
            for(auto single_obs : observation){
		
		auto start = std::chrono::steady_clock::now();
                if (mmap)
                    blocks = mmapAndPredict(single_obs, preds, ct);
                else{
                    blocks = predict(single_obs, preds);
                }
		std::cout<<"****************************\n";
		//std::cout<<"after observation: "<<ct<<"\n";
                num_blocks.push_back(blocks);
                results.push_back((double)preds[0] / (double)preds[1] );    
            
		auto end = std::chrono::steady_clock::now();
#ifdef LAT_LOGGING
		double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
		elapsed_arr.push_back(elapsed);
#endif
		ct+=1;
		if(ct==-2)
			break;
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
            int num_classes = std::stoi(Config::getValue("numclassesactual"));
            int num_bins = bins.size();
            std::vector<int> bin_sizes = PacsetBaseModel<T, F>::bin_sizes;
            std::vector<int> bin_node_sizes = PacsetBaseModel<T, F>::bin_node_sizes;
            std::vector<std::vector<int>> bin_start  = PacsetBaseModel<T, F>::bin_start;
            std::string format = Config::getValue("format");
            
            //Write the metadata needed to reconstruct bins and for prediction
            //TODO: change filename
	    std::string filename;
	    if(Config::getValue("metadatafilename") == std::string("notfound"))
	    	filename = "metadata.txt";
            else
		filename = Config::getValue("metadatafilename"); 
	    
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

                std::string modelfname = Config::getValue("packfilename");
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
                        fout.write((char*)&node_to_write, sizeof(node_to_write));
                    }
                }
                fout.close();

            }
            else{
                //Write the nodes
                std::string modelfname = Config::getValue("packfilename");
                std::string filename;

		if(modelfname != std::string("notfound"))
		    filename = modelfname;
		else
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
	    std::cout<<num_classes<<"\n";
	    //Number of bins
            f>>num_bins;
            Config::setConfigItem("numthreads", std::to_string(num_bins));
	    std::cout<<num_bins<<"\n";
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

};

#endif
