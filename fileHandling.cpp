#include "fileHandling.hpp"

PGMFile::PGMFile(std::string filename){
    std::streampos size;
    std::string pgm_type, dimension, levels;
    std::string line;
    std::ifstream myfile (filename);
    int image_begin;
    if (myfile.is_open())
    {
        getline (myfile, this->pgm_type);
        getline (myfile, this->dimensions);
        getline (myfile, this->levels);
        myfile.close();
    }
    
    image_begin = pgm_type.size() + dimension.size() + levels.size() + 3;
    std::ifstream file (filename, std::ios::in|std::ios::binary|std::ios::ate);

    if (file.is_open())
    {
        size = file.tellg();
        this->image_data = new char [size];
        file.seekg (image_begin, std::ios::beg);
        file.read (this->image_data, size);
        file.close();
    }
}

std::string PGMFile::getPgmType(){
    return this->pgm_type;
}

std::string PGMFile::getDimensions(){
    return this->dimensions;
}

std::string PGMFile::getLevels(){
    return this->levels;
}

char* PGMFile::getImageData(){
    return this->image_data;
}