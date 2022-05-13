#include<iostream>
#include "fileHandling.hpp"

int main(){
    std::string filename = "treino/lena.256.pgm";
    PGMFile image(filename);
    std::string image_data = std::string(image.getImageData());
    std::ofstream myfile;
    int coefficient;
    myfile.open ("lena.csv");
    for (unsigned int i = 0; i < image_data.size(); i++)
        if (i % 2 == 0) {
            coefficient = image_data.at(i);
            if (coefficient < 0)
                coefficient += 255;
            myfile << coefficient << ",";
        }
        else {
            coefficient = image_data.at(i);
            if (coefficient < 0)
                coefficient += 255;
            myfile << coefficient << '\n';

        }
    myfile.close();
    return 0;

}