#include "fileHandling.hpp"

PGMFile::PGMFile(std::string filename){
    std::streampos size;
    std::string pgm_type, dimension, levels;
    std::string line;
    std::ifstream myfile (filename);
    //int image_begin;
    if (myfile.is_open())
    {
        getline (myfile, this->pgm_type);
        getline (myfile, this->dimensions);
        getline (myfile, this->levels);
        myfile.close();
    }
    
    //image_begin = pgm_type.size() + dimension.size() + levels.size() + 3;
    std::ifstream file (filename, std::ios::binary);
/*
    if (file.is_open())
    {
        size = file.tellg();
        this->image_data = new char [size];
        file.seekg (15, std::ios::beg); //1s is hardcoded, each image can have a different image begin value. Should refactor
        file.read (this->image_data, size);
        file.close();
    }
*/
    /*Open the stream in binary mode.*/
    std::ifstream bin_file(filename, std::ios::binary);

    if (bin_file.good()) {
        /*Read Binary data using streambuffer iterators.*/
        std::vector<uint8_t> v_buf((std::istreambuf_iterator<char>(bin_file)), (std::istreambuf_iterator<char>()));
        this->image_data = v_buf;
        bin_file.close();
    }


    
}

std::string PGMFile::getPgmType(){
    return this->pgm_type;
}

std::string PGMFile::getDimensions(){
    return this->dimensions;
}

int PGMFile::getWidth(){
    std::string width = "";
    for (auto el : this->dimensions){
        if(el != ' ')
            width += el;
        else
            break;
    }
    return std::stoi(width);
}

int PGMFile::getHeight(){
    std::string height = "";
    int space_flag = 0;
    for (auto el : this->dimensions) {
        if (space_flag)
            height += el;
        if(el == ' ')
            space_flag = 1;
    }
    return std::stoi(height);
}

std::string PGMFile::getLevels(){
    return this->levels;
}

std::vector<uint8_t> PGMFile::getImageData(){
    return this->image_data;
}

void reshapeVector(std::vector<std::vector<int>> &matrix, std::vector<int> vec, int K, int L){
    for (int i = 0; i < K * L; i++)
        matrix.at(i / L).at(i % L) = vec.at(i);
}
