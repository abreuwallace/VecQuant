#include<iostream>
#include<random>
#include<cmath>
#include<algorithm>
#include<stdint.h>
#include "fileHandling.hpp"
#include "KMeans.hpp"
int main(){
    std::vector<std::string> files = {"treino/lena.256.pgm", "treino/aerial.pgm"};
    std::string filename = "treino/lena.256.pgm";
    PGMFile image(filename);
    std::string image_data = std::string(image.getImageData());
    uint8_t coefficient;
    int M = 256;
    int N = 256;
    int M_q = M;
    int N_q = N;
    std::vector<int> matrix_line;
    std::vector<std::vector<int>> matrix;
    int line_count = 0;
    int K = 2;
    int L = 4;
    std::vector<std::vector<int>> block_list (M / K, std::vector<int> (N / L));
    //Matrix of image chars building
    for (unsigned int i = 0; i < image_data.size(); i++){
        coefficient = image_data.at(i);
        //std::cout << coefficient << " ";
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

    std::vector<int> flat_block;
    std::vector<std::vector<int>> flat_blocks_vector ((M / K) * (N / L));

    std::vector<std::vector<int>> block (M / K, std::vector<int> (N / L));
    int idx = 0;
    //Block flattening procedure
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

    int n_clusters = 128;

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, 255);
    std::vector<int> Y_vector;
    std::vector<std::vector<int>> Y (n_clusters);
    //std::normal_distribution<double> g_distribution(0, 1);

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
    //Kmeans++ init
    std::uniform_int_distribution<> first_center_dist(0, N - 1);
    int first_center_idx = first_center_dist(generator);
    Y.at(0) = flat_blocks_vector.at(first_center_idx);
    for (int n = 1; n < n_clusters; n++){
        std::vector<int> mean_center (M, 0);
        std::vector<double> distance_from_mean(N);
        for (int j = 0; j < n; j++)
            mean_center = DivideVectorByScalar(SumVectors(mean_center, Y.at(j)), n);
        for (int v = 0; v < N; v++)
            distance_from_mean.at(v) = EuclideanNorm(SubtractVectors(mean_center, flat_blocks_vector.at(v)));

        std::discrete_distribution<int> distribution (distance_from_mean.begin(), distance_from_mean.end());
        Y.at(n) = flat_blocks_vector.at(distribution (generator));
    }

    int running = 1;
    int i = 0;
    int iterations = 20;
    double delta = 0.001;
    std::vector<double> D (iterations);

    std::vector<int> labels (N);
    int index;
    std::vector<double> distances (n_clusters, 0);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Kmeans loop
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
            std::vector<int> vec (M, 0);
            for (int t = 0; t < N; t++) {
                if (labels.at(t) == n)
                    vec = SumVectors(vec, flat_blocks_vector.at(t));
            }
            Y.at(n) = DivideVectorByScalar(vec, 1 + nOfLabelsEqualToIndex(labels, n));
            if (nOfLabelsEqualToIndex(labels, n) == 0)
                Y.at(n) = flat_blocks_vector.at(distribution (generator)); //Should use a better strategy like choosing (randomly) a vector from the most populous label
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
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Associate quantized vector to cluster labels
    std::vector<std::vector<int>> quantized_flat_blocks_vector (N);
    for (int i = 0; i < N; i++)
        quantized_flat_blocks_vector.at(i) = Y.at(labels.at(i));

    std::vector<std::vector<int>> quantized_matrix (M_q, std::vector<int> (N_q));
    std::vector<std::vector<int>> quantized_block(K, std::vector<int> (L));
    std::vector<std::vector<std::vector<int>>> quantized_blocks_vector;

    //Initialize vector of quantized blocks
    for (auto vec : quantized_flat_blocks_vector){
        for (int i = 0; i < K * L; i++) {
            quantized_block.at(i / L).at(i % L) = vec.at(i);
        }
        quantized_blocks_vector.push_back(quantized_block);
    }   

    //Construct full matrix from blocks
    int block_index = 0;
    for (int i = 0; i < M_q; i += K){
        for (int j = 0; j < N_q; j+=L){
            for (int k = i; k < i + K; k++){
                for (int l = j; l < j + L; l++){
                    quantized_matrix.at(k).at(l) = quantized_blocks_vector.at(block_index).at(k % K).at(l % L);
                }
            }
            block_index++;
        }
    }
    std::ofstream qt_img ("lena_qnt.pgm", std::ios::binary );
    /*
    if (qt_img.is_open())
    {
        qt_img << image.getPgmType() << '\n';
        qt_img << image.getDimensions() << '\n';
        qt_img << image.getLevels() << '\n';
        for (int n = 0; n < M_q; n++) {
            for (int t = 0; t < N_q; t++) {
                std::cout << quantized_matrix.at(n).at(i);
                if (quantized_matrix.at(n).at(i) >= 256) {
                    qt_img << static_cast<char>(quantized_matrix.at(n).at(i));
                }
                else {
                    qt_img << static_cast<char>(quantized_matrix.at(n).at(i) - 256);
                }
            }
        }
        qt_img.close();
    }
    */
    if (qt_img.is_open()) {
        qt_img << image.getPgmType() << '\n';
        qt_img << image.getDimensions() << '\n';
        qt_img << image.getLevels() << '\n';
        for (int i = 0; i < M_q; i++){
            for (int j = 0; j < N_q; j++){
                qt_img << static_cast<char>(quantized_matrix.at(i).at(j));
            }
        }
    }

    std::cout << "PSNR: " << PSNR(matrix, quantized_matrix, M_q, N_q);
    std::ofstream myfile ("lena.csv");
    if (myfile.is_open())
    {
        myfile << "x1, x2" << '\n';
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

