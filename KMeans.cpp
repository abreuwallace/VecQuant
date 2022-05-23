#include "KMeans.hpp"

double EuclideanNorm(std::vector<int> v){
    double norm = 0;
    for (auto el : v)
        norm += std::pow(el, 2);
    return norm;
}

std::vector<int> SubtractVectors(std::vector<int> v, std::vector<int> w){
    if (v.size() != w.size())
        throw std::invalid_argument("Both vectors must have the same size");
    std::vector<int> vec;
    for (unsigned int i = 0; i < v.size(); i++)
        vec.push_back(v.at(i) - w.at(i));
    return vec;
}

std::vector<int> SumVectors(std::vector<int> v, std::vector<int> w){
    if (v.size() != w.size())
        throw std::invalid_argument("Both vectors must have the same size");
    std::vector<int> vec;
    for (unsigned int i = 0; i < v.size(); i++)
        vec.push_back(v.at(i) + w.at(i));
    return vec;
}

std::vector<int> DivideVectorByScalar(std::vector<int> v, double n){
    std::vector<int> vec;
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

double PSNR(std::vector<std::vector<int>> X, std::vector<std::vector<int>> X_hat, int M, int N){
    double mse = 0;
    double psnr;
    for (int m = 0; m < M; m++){
        for (int n = 0; n < N; n++){
            mse += std::pow(X.at(m).at(n) - X_hat.at(m).at(n), 2);
        }
    }
    mse /= (M * N);
    if (mse > 0)
        psnr = 10 * std::log10(65025 / mse);
    else 
        psnr = 100;
    return psnr;
}
