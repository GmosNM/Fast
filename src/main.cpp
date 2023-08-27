#include "parser.hpp"

auto print_usage() -> void {
    std::cerr << "Usage: fast [init|build|update]" << std::endl;
    return;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        print_usage();
        return 1;
    }

    Fast fast;
    Parser parser;

    if (strcmp(argv[1], "init") == 0) {
        fast.init();
        fast.setExecutableName(fast.get_fast_project_name());
    } else if (strcmp(argv[1], "build") == 0) {
        parser.parse(".fast");
        fast.setCompiler(parser.getCompilerName());
        fast.setExecutableName(parser.getExecutableName());
        fast.BuildFastProject();
    } else if (strcmp(argv[1], "update") == 0) {
        parser.parse(".fast");
        fast.setCompiler(parser.getCompilerName());
        fast.setExecutableName(parser.getExecutableName());
        fast.UpdateTheFastFile();
    } else if (strcmp(argv[1], "run") == 0) {
        parser.parse(".fast");
        fast.setCompiler(parser.getCompilerName());
        fast.setExecutableName(parser.getExecutableName());
        fast.BuildFastProject();
        fast.RunFast();
    }else if (strcmp(argv[1], "test") == 0) {
    } else {
        std::cerr << "Wrong argument" << std::endl;
        print_usage();
        return 1;
    }
}
