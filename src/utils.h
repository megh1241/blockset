#include <iostream>
#include <stdlib.h>
#include <vector>


void loadTestData(std::vector<std::vector<float>>& test_data, std::vector<int>& labels);
void loadTestData(std::vector<std::vector<float>>& test_data, std::vector<double>& labels);
double getAccuracy(const std::vector<int> &predicted, const std::vector<int> &labels);
double getAccuracy(const std::vector<double> &predicted, const std::vector<double> &labels);
