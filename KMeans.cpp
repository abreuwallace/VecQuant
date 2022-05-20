#include "KMeans.hpp"

double EuclideanNorm(std::vector<double> v){
    double norm = 0;
    for (auto el : v)
        norm += std::pow(el, 2);
    return norm;
}

std::vector<double> SubtractVectors(std::vector<double> v, std::vector<double> w){
    if (v.size() != w.size())
        throw std::invalid_argument("Both vectors must have the same size");
    std::vector<double> vec;
    for (unsigned int i = 0; i < v.size(); i++)
        vec.push_back(v.at(i) - w.at(i));
    return vec;
}

std::vector<double> SumVectors(std::vector<double> v, std::vector<double> w){
    if (v.size() != w.size())
        throw std::invalid_argument("Both vectors must have the same size");
    std::vector<double> vec;
    for (unsigned int i = 0; i < v.size(); i++)
        vec.push_back(v.at(i) + w.at(i));
    return vec;
}

std::vector<double> DivideVectorByScalar(std::vector<double> v, double n){
    std::vector<double> vec;
    for (unsigned int i = 0; i < v.size(); i++)
        vec.push_back(v.at(i) / n);
    return vec;
}

int nOfLabelsEqualToIndex(std::vector<int> labels, int index){
    int count = 0;
    for (auto el : labels)
        if (el == index)
            count++;
    return count;
}

int findVectorArgmin(std::vector<double> v){
    return std::distance(v.begin(), std::min_element(v.begin(), v.end()));
}


/*
void InitializeClusterVectors(std::vector<std::vector<double>> &matrix, std::vector<double> &vec, int n_clusters, int K, int L){
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, 255);
    for (int i = 0; i < n_clusters; i++){
        for (int j = 0; j < K * L; j++)
            vec.push_back(distribution(generator));
        matrix.at(i) = vec;
        vec.clear();
    }
}

void InitializePivotVector(std::vector<double> &pivot, std::vector<std::vector<int>> flat_blocks, int vec_index, int n_features){
    for (int t = 0; t < n_features; t++)
        pivot.at(t) = flat_blocks.at(vec_index).at(t);
}
*/