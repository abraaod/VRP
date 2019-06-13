#include "../include/vrp_CW.hpp"

int main(int argc, char const * argv[]){

    /*std::vector<std::string> files;
    for(int i = 1; i < argc; i++){
        files.push_back(argv[i]);
    }*/
    //std::copy(files.begin(), files.end(), std::ostream_iterator<std::string>(std::cout, " "));
    std::string file = argv[1];
    vrp_CW v(file);
}