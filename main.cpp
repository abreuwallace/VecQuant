#include<iostream>
#include<random>
#include<cmath>
#include<algorithm>
#include "fileHandling.hpp"
#include "KMeans.hpp"
int main(){
    std::string filename = "treino/lena.256.pgm";
    PGMFile image(filename);
    std::string image_data = std::string(image.getImageData());
    int coefficient;
    int M = 256;
    int N = 256;
    std::vector<int> matrix_line;
    std::vector<std::vector<int>> matrix;
    int line_count = 0;
    int K = 1;
    int L = 2;
    std::vector<std::vector<int>> block_list (M / K, std::vector<int> (N / L));
    for (unsigned int i = 0; i < image_data.size(); i++){
        coefficient = image_data.at(i);
        if (coefficient < 0)
            coefficient += 256;

        matrix_line.push_back(coefficient);
        if (line_count == N - 1) {
            matrix.push_back(matrix_line);
            //for (auto el : matrix_line)
            //    std::cout << el << ' ';
            matrix_line.clear();
            line_count = 0;
        }
        else 
            line_count++;
    }
    //std::cout << '\n';
/*

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            std::cout << matrix.at(i).at(j) << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
*/

    std::vector<double> flat_block;
    std::vector<std::vector<double>> flat_blocks_vector ((M / K) * (N / L));

    std::vector<std::vector<int>> block (M / K, std::vector<int> (N / L));
    int idx = 0;
    for (int i = 0; i < M; i += K){
        for (int j = 0; j < N; j += L){
            for (int k = i; k < i + K; k++){
                for (int l = j; l < j + L; l++) {
                    //std::cout << matrix.at(k).at(l) << ' ';
                    flat_block.push_back(matrix.at(k).at(l));
                    if (k == i + K - 1 && l == j + L - 1){
                        flat_blocks_vector.at(idx) = flat_block;
                        idx++;
                    }      
                }
                //std::cout << '\n';
            }
            flat_block.clear();
            //std::cout << '\n';
        }
    }
    //for (auto el : flat_blocks_vector.at(M * N / (K * L) - 1))
    //    std::cout << el << ' ';
    
    //Should create another 2 variables for these dimensions
    N = flat_blocks_vector.size(); 
    M = flat_blocks_vector.at(0).size();

    int n_clusters = 16;

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, 255);
    std::vector<double> Y_vector;
    std::vector<std::vector<double>> Y (n_clusters);
    std::normal_distribution<double> g_distribution(0, 1);

    //Initialize cluster vectors
    for (int i = 0; i < n_clusters; i++){
        for (int j = 0; j < K * L; j++)
            Y_vector.push_back(distribution(generator));
        Y.at(i) = Y_vector;
        Y_vector.clear();
        //for (auto el : Y.at(i))
        //    std::cout << el << " ";
        //std::cout << '\n';
    }
    std::ofstream myfile ("lena.csv");

    int running = 1;
    int i = 0;
    int iterations = 30;
    double delta = 0.01;
    std::vector<double> D (iterations);
    /*
    while(running){


        if (i == iterations - 1)
            running = 0;
        i++;
    }
    */
    std::vector<int> labels (N);
    int index;
    std::vector<double> distances (n_clusters, 0);
    while (running){
        std::cout << "Iteration: " << i << '\n';
        for (int v = 0; v < N; v++){
            for (int n = 0; n < n_clusters; n++) {
                distances.at(n) = EuclideanNorm(SubtractVectors(flat_blocks_vector.at(v), Y.at(n)));
                //std::cout << distances.at(n) << ' ';
            }
            //std::cout << '\n';  
            index = findVectorArgmin(distances);
            labels.at(v) = index;
            //for (auto el : Y.at(index))
            //    std::cout << el << ' ';
            //std::cout << '\n';  
            //for (auto el : Y.at(index))
            //    std::cout << el << ' ';
        }

        for (int n = 0; n < n_clusters; n++) {
            std::vector<double> vec (M, 0);
            for (int t = 0; t < N; t++) {
                if (labels.at(t) == n)
                    vec = SumVectors(vec, flat_blocks_vector.at(t));
            }
            Y.at(n) = DivideVectorByScalar(vec, 1 + nOfLabelsEqualToIndex(labels, n));
        }

        for (auto el : distances)
            D.at(i) += el;
        std::cout << "D: " << D.at(i) << "\n";
        if (i > 0 && std::abs((D.at(i) - D.at(i - 1)) / D.at(i - 1)) < delta)
            running = 0;
        if (i == iterations - 1)
            running = 0;
        i++;
    }
    /*
    int i = 0; //should change this for a better name
    double delta = 0.001;
    double epsilon = 0.001;
    int I = 10;
    std::vector<double> D (I, 0);
    int end = 0;
    double d = 0;
    double distance = 0;
    int index;
    int max_index;
    std::vector<double> distances_vector (n_clusters);
    std::vector<double> aux (M, 0); 
    
    std::vector<int> labels (N, n_clusters);
    while (!end) {
        std::vector<double> p (n_clusters, 0); 
        //Partition Condition
        for (int n = 0; n < N; n++) {

            //Initialize pivot vector
            for (int t = 0; t < M; t++)
                aux.at(t) = flat_blocks_vector.at(n).at(t);

            //Calculate distance from data vector to cluster vector
            for (int l = 0; l < n_clusters; l++) {
                for (int t = 0; t < M; t++)
                    d += EuclideanNorm(SubtractVectors(aux, Y.at(l)));
                distance += d;
                distances_vector.at(l) = d;
                d = 0; 
            }
            //Get index of the cluster vector that is closer to the chosen data vector
            index = std::distance(distances_vector.begin(), std::min_element(distances_vector.begin(), distances_vector.end()));
            
            labels.at(n) = index;
            p.at(index)++;
            
            D.at(i) += distances_vector.at(index);
            
        }
        D.at(i) = D.at(i) / N;
  
        int label_count = 0;
        std::cout << D.at(i) << "\n";
        for (int v = 0; v < N; v++)
            for (int n = 0; n < n_clusters; n++){
                if (labels.at(v) == n) {
                    for (int k = 0; k < N; k++)
                        if (labels.at(v) == n)
                            label_count++;
                    for (int t = 0; t < M; t++)
                        Y.at(n).at(t) = (Y.at(n).at(t) + flat_blocks_vector.at(v).at(t)) / label_count;
                }   
                label_count = 0;
                
            }
        if (i > 0) {
            if ((D.at(i) - D.at(i - 1)) / D.at(i - 1) < delta)
                end = 1;
        }

        if (i == I - 1)
            end = 1;
        i++;
        
    }
    */
    if (myfile.is_open())
    {
        for (int n = 0; n < n_clusters; n++) {
            for (int t = 0; t < M; t++) {
                if (t != M - 1)
                    myfile << Y.at(n).at(t) << ',';
                else
                    myfile << Y.at(n).at(t);
            }
            myfile << '\n';
        }
        myfile.close();
    }

    return 0;

}

