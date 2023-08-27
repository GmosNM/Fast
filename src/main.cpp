#include "fast.hpp"

auto print_useage() -> void {
    std::cerr << "Usage: fast [init|build|update]" << std::endl;
    return;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        print_useage();
        return 1;
    }
    Fast fast;

    if (strcmp(argv[1], "init") == 0) {
        fast.init();
    } else if (strcmp(argv[1], "build") == 0) {
        fast.BuildFastProject();
    }else if (strcmp(argv[1], "update") == 0) {
        fast.UpdateTheFastFile();
        fast.KeepTrackOfModules();
    }else{
        std::cerr << "Wrong argument" << std::endl;
        return 1;
    }
}
