#include <iostream>
#include <stdlib.h>
#include <vector>


void loadTestData(std::vector<std::vector<float>>& test_data, std::vector<int>& labels);
double getAccuracy(std::vector<int> predicted, std::vector<int> labels);
