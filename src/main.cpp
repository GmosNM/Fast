#include "fast.hpp"


int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Wrong number of arguments" << std::endl;
        return 1;
    }
    Fast fast;

    if (strcmp(argv[1], "init") == 0) {
        fast.init();
    } else if (strcmp(argv[1], "build") == 0) {
        fast.BuildFastProject();
    }else if (strcmp(argv[1], "update") == 0) {
        fast.UpdateTheFastFile();
    }else{
        std::cerr << "Wrong argument" << std::endl;
        return 1;
    }
}
