#include "utils.h"
#include "config.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>  

void loadTestData(std::vector<std::vector<float>>& test_data, std::vector<int>& labels){
    std::string filename = Config::getValue("datafilename");
    std::fstream fin;
    fin.open(filename, std::ios::in);
    std::vector<std::string> row; 
    std::string line, word, temp; 
    std::vector<float> temp_vector;
    int num_obs = 0;
    int max_n = 1000;
    while(getline(fin, line, '\n')){
        std::istringstream templine(line);
        std::string data;
        while(getline(templine, data, ',')){
            temp_vector.push_back(std::atof(data.c_str()));
        }
        int siz = temp_vector.size();
        int last_ele = (int)(temp_vector.at(siz-1));
        labels.push_back(last_ele);
        temp_vector.pop_back();
        test_data.push_back(temp_vector);
        temp_vector.clear();
        num_obs++;
        if (num_obs > max_n)
            break;
    }
    fin.close();
}

void loadTestData(std::vector<std::vector<float>>& test_data, std::vector<double>& labels){
    std::cout<<"loading test data\n";
    std::string filename = Config::getValue("datafilename");
    std::fstream fin;
    fin.open(filename, std::ios::in);
    std::vector<std::string> row; 
    std::string line, word, temp; 
    std::vector<float> temp_vector;
    while(getline(fin, line, '\n')){
        std::istringstream templine(line);
        std::string data;
        while(getline(templine, data, ',')){
            temp_vector.push_back(std::atof(data.c_str()));
        }
        int siz = temp_vector.size();
        double last_ele = (double)(temp_vector.at(siz-1));
        labels.push_back(last_ele);
        temp_vector.pop_back();
        test_data.push_back(temp_vector);
        temp_vector.clear();
    }
    std::cout<<"size of test data: "<<test_data.size()<<"\n";
    fin.close();
}

double getAccuracy(const std::vector<int> &predicted, const std::vector<int> &labels){
    int wrong = 0;
    int siz = predicted.size();
    for(int i=0; i<siz; ++i){
        if(labels[i] != predicted[i]){
            std::cout<<"labels[i]: "<<labels[i]<<"\n";
            std::cout<<"predicted[i]: "<<predicted[i]<<"\n";
            wrong++;
        }
    }
    std::cout<<"Number wrong!: "<<wrong<<"\n";
    std::cout<<"total num: "<<predicted.size()<<"\n";
    return (double)(siz - wrong) / (double)(siz);
}

double getAccuracy(const std::vector<double> &predicted, const std::vector<double> &labels){
    int wrong = 0;
    int siz = predicted.size();
    double diff, sum;
    sum = 0;
    std::cout<<"Label size: "<<labels.size()<<"\n";
    std::cout<<"Size: "<<siz<<"\n";
    for(int i=0; i<siz; ++i){
        diff = std::pow(predicted[i] - labels[i], 2);
        sum += diff;
    }
    double mean_square = std::sqrt(sum) / (double)(siz);
    return mean_square;
}
