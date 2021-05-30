#include <iostream>
#include <stdlib.h>
#include <vector>

float logit(float input);
float logit2(float input);
std::vector<std::vector<float>> logit(std::vector<std::vector<float>> input_mat);
std::vector<float> logit(std::vector<float> input_mat);
void loadTestData(std::vector<std::vector<float>>& test_data, std::vector<int>& labels);
void loadTestData(std::vector<std::vector<float>>& test_data, std::vector<double>& labels);
double getAccuracy(const std::vector<int> &predicted, const std::vector<int> &labels);
double getAccuracy(const std::vector<double> &predicted, const std::vector<double> &labels);
