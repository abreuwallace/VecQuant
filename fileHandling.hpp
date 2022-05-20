#include<fstream>
#include<string>
#include<vector>
class PGMFile {
    public:
        PGMFile(std::string);
        std::string getPgmType();
        std::string getDimensions();
        std::string getLevels();
        char* getImageData();

    private:
        std::string pgm_type;
        std::string dimensions;
        std::string levels;
        char* image_data;

};

