#include<iostream>
#include<random>
#include<cmath>
#include<algorithm>
#include<stdexcept>

double EuclideanNorm(std::vector<double> );
std::vector<double> SubtractVectors(std::vector<double>, std::vector<double>);
std::vector<double> SumVectors(std::vector<double>, std::vector<double>);
std::vector<double> DivideVectorByScalar(std::vector<double>, double );
int nOfLabelsEqualToIndex(std::vector<int>, int);
int findVectorArgmin(std::vector<double>);