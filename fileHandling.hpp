#include<fstream>
#include<string>
#include<vector>
#include<iostream>
class PGMFile {
    public:
        PGMFile(std::string);
        std::string getPgmType();
        std::string getDimensions();
        int getHeight();
        int getWidth();
        std::string getLevels();
        std::vector<uint8_t> getImageData();

    private:
        std::string pgm_type;
        std::string dimensions;
        std::string levels;
        //char* image_data;
        std::vector<uint8_t> image_data;

};

void reshapeVector(std::vector<std::vector<int>> &, std::vector<int>, int, int);
