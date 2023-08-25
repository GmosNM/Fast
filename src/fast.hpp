#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <unordered_set>
#include <cstring>

class Fast {
public:
    Fast() {
        std::vector<std::string> currentFiles = CleanDirVectorPath("./src");
        src_fast_files = std::unordered_set<std::string>(currentFiles.begin(), currentFiles.end());
        fast_project_name = GetProjectNameFromTheDirName();
    }

    auto init() -> void;
    auto WalkIntoDir(std::string dir) -> std::vector<std::string>;
    auto CleanDirVectorPath(std::string dir) -> std::vector<std::string>;
    auto UpdateTheFastFile() -> void;
    auto GetProjectNameFromTheDirName() -> std::string;
    auto GetDirName() -> std::string;
    auto BuildFastProject() -> void;

    auto GetFileNameWithoutExtension(const std::string& file);
    auto GetFileExtension(const std::string& file);

private:
    std::string fast_file = ".fast";
    std::string fast_version = "0.1";
    std::string fast_project_name;
    std::string fast_compiler_name = "clang++ ";
    std::unordered_set<std::string> src_fast_files = {};
    std::unordered_set<std::string> fast_modules = {};
};
