#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_set>
#include <cstring>
#include <fstream>
#include <sstream>


#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#endif

class Fast {
    public:
        Fast() {
            std::vector<std::string> currentFiles = CleanDirVectorPath("./src");
            src_fast_files = std::unordered_set<std::string>(currentFiles.begin(), currentFiles.end());
            fast_project_name = GetProjectNameFromTheDirName();
            GenrateModuleDir();
        }

        auto init() -> void;
        auto WalkIntoDir(const std::string& dir) -> std::vector<std::string> ;
        auto CleanDirVectorPath(const std::string& dir) -> std::vector<std::string> ;
        auto UpdateTheFastFile() -> void;
        auto GetProjectNameFromTheDirName() -> std::string;
        auto GetDirName() -> std::string;
        auto BuildFastProject() -> void;

        // Modules
        auto GenrateModuleDir() -> void;
        auto KeepTrackOfModules() -> std::unordered_set<std::string> ;

        auto GetFileNameWithoutExtension(const std::string& file);
        auto GetFileExtension(const std::string& file);

        // Helpers
        auto CreateDirectory_(const std::string& dir) -> bool;
        auto GetCurrentDirName() -> std::string;
        auto DirectoryExists(const std::string& dir) -> bool;


        // Getters
        auto get_fast_file() -> std::string{
            return fast_file;
        }

        auto get_fast_version() -> std::string{
            return fast_version;
        }

        auto get_fast_project_name() -> std::string{
            return fast_project_name;
        }

        auto get_fast_compiler_name() -> std::string{
            return fast_compiler_name;
        }

        // Setters

        auto set_fast_file(std::string file){
            fast_file = file;
        }

        auto set_fast_version(std::string version){
            fast_version = version;
        }

        auto set_fast_project_name(std::string project_name){
            fast_project_name = project_name;
        }

        auto set_fast_compiler_name(std::string compiler_name){
            fast_compiler_name = compiler_name;
        }

        auto setExecutableName(std::string executable_name){
            fast_executable_name = executable_name;
        }

        auto setCompiler(std::string compiler_name){
            fast_compiler_name = compiler_name;
        }

    private:
        std::string fast_file = ".fast";
        std::string fast_version = "0.1";
        std::string fast_project_name;
        std::string fast_compiler_name = "";
        std::string fast_executable_name = "";
        std::unordered_set<std::string> fast_object_files = {};
        std::unordered_set<std::string> src_fast_files = {};
        std::unordered_set<std::string> fast_modules = {};
};
