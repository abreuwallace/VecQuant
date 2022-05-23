#include<iostream>
#include<random>
#include<cmath>
#include<algorithm>
#include<stdexcept>

double EuclideanNorm(std::vector<int> );
std::vector<int> SubtractVectors(std::vector<int>, std::vector<int>);
std::vector<int> SumVectors(std::vector<int>, std::vector<int>);
std::vector<int> DivideVectorByScalar(std::vector<int>, double );
int nOfLabelsEqualToIndex(std::vector<int>, int);
int findVectorArgmin(std::vector<double>);
double PSNR(std::vector<std::vector<int>>, std::vector<std::vector<int>>, int, int);