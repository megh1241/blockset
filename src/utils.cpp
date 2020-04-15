#include "utils.h"
#include "config.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


void loadTestData(std::vector<std::vector<float>>& test_data, std::vector<int>& labels){
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
        labels.push_back(temp_vector.back());
        temp_vector.pop_back();
        test_data.push_back(temp_vector);
        temp_vector.clear();
    }
    fin.close();
}

double getAccuracy(std::vector<int> predicted, std::vector<int> labels){
    int wrong = 0;
    int siz = predicted.size();
    for(int i=0; i<siz; ++i)
        if(labels[i] != predicted[i])
            wrong++;

    return (double)(siz - wrong) / (double)(siz);
}
