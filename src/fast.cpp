#include "fast.hpp"

void Fast::init() {
    std::string filename = ".fast";
    std::fstream file(filename, std::ios::in);
    fast_compiler_name = "clang++";

    if (!file) {
        std::cout << "Creating file: " << filename << std::endl;
        std::ofstream outfile(filename);
        outfile.close();
        UpdateTheFastFile();
    } else {
        std::cout << "File already exists: " << filename << std::endl;
        file.close();
    }
}

auto Fast::WalkIntoDir(const std::string& dir) -> std::vector<std::string> {
    std::vector<std::string> files;

#ifdef _WIN32
    std::string searchPath = dir + "/*";
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
                std::string filePath = dir + "/" + findData.cFileName;
                files.push_back(filePath);
            }
        } while (FindNextFileA(hFind, &findData) != 0);

        FindClose(hFind);
    }
#else
    DIR* directory = opendir(dir.c_str());
    if (directory != nullptr) {
        struct dirent* entry;
        while ((entry = readdir(directory)) != nullptr) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                std::string filePath = dir + "/" + entry->d_name;
                files.push_back(filePath);
            }
        }
        closedir(directory);
    }
#endif

    return files;
}

auto Fast::CleanDirVectorPath(const std::string& dir) -> std::vector<std::string> {
    if (!DirectoryExists(dir)) {
        std::cout << "Creating folder: " << dir << std::endl;
        Fast::CreateDirectory_(dir);
    }

    std::vector<std::string> files = WalkIntoDir(dir);
    for (auto& file : files) {
        // Remove "./" occurrences
        size_t pos = file.find("./");
        while (pos != std::string::npos) {
            file.erase(pos, 2);
            pos = file.find("./");
        }

        // Replace backslashes with forward slashes
        size_t backslashPos = file.find("\\");
        while (backslashPos != std::string::npos) {
            file.replace(backslashPos, 1, "/");
            backslashPos = file.find("\\", backslashPos + 1);
        }
    }

    std::sort(files.begin(), files.end());
    return files;
}

auto Fast::UpdateTheFastFile() -> void {
    std::unordered_set<std::string> fast_modules = KeepTrackOfModules();
    std::fstream _fast_file(Fast::fast_file, std::ios::out);
    _fast_file << "VERSION: " << Fast::fast_version << std::endl;
    _fast_file << "PROJECT: " << Fast::fast_project_name << std::endl;
    _fast_file << "EXECUTABLE: " << Fast::fast_executable_name << std::endl;
    _fast_file << "COMPILER: " << Fast::fast_compiler_name << std::endl;
    _fast_file << "\n\nFILES:" << std::endl;
    for (const auto& file : src_fast_files) {
        _fast_file << "   " << file << std::endl;
    }
    _fast_file << "\n\nMODULES:" << std::endl;
    for (const auto& module : fast_modules) {
        _fast_file << "   " << module << std::endl;
    }

    _fast_file.close();
}



auto Fast::GetCurrentDirName() -> std::string {
    std::string currentPath;
#ifdef _WIN32
    const DWORD bufferSize = MAX_PATH;
    char buffer[bufferSize];
    DWORD result = ::GetCurrentDirectoryA(bufferSize, buffer);
    if (result != 0 && result < bufferSize) {
        currentPath = buffer;
    }
#else
    const int bufferSize = PATH_MAX;
    char buffer[bufferSize];
    if (getcwd(buffer, bufferSize) != nullptr) {
        currentPath = buffer;
    }
#endif

    // Extract only the directory name from the full path
    size_t lastSlash = currentPath.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        currentPath = currentPath.substr(lastSlash + 1);
    }

    return currentPath;
}


bool Fast::DirectoryExists(const std::string& dir) {
#ifdef _WIN32
    DWORD attrib = GetFileAttributesA(dir.c_str());
    return (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY));
#else
    struct stat info;
    if (stat(dir.c_str(), &info) != 0)
        return false;
    return S_ISDIR(info.st_mode);
#endif
}

auto Fast::GetDirName() -> std::string {
    return GetCurrentDirName();
}

std::string Fast::GetProjectNameFromTheDirName() {
    std::string dirName = GetDirName();
    size_t pos = dirName.find_last_of("//");
    fast_project_name = dirName.substr(pos + 1);
    return fast_project_name;
}

auto Fast::GetFileNameWithoutExtension(const std::string& file) {
    size_t lastSlash = file.find_last_of("/\\");
    size_t lastDot = file.find_last_of(".");
    if (lastSlash != std::string::npos && lastDot != std::string::npos && lastDot > lastSlash) {
        return file.substr(lastSlash + 1, lastDot - lastSlash - 1);
    }
    return file;
}

auto Fast::GetFileExtension(const std::string& file) {
    size_t lastDot = file.find_last_of(".");
    if (lastDot != std::string::npos) {
        return file.substr(lastDot + 1);
    }
    return file.substr(lastDot + 1);
}

auto Fast::GenrateModuleDir() -> void {
    CreateDirectory_("Modules");
}

auto Fast::KeepTrackOfModules() -> std::unordered_set<std::string> {
    std::vector<std::string> modules = CleanDirVectorPath("Modules");
    return std::unordered_set<std::string>(modules.begin(), modules.end());
}

auto Fast::CreateDirectory_(const std::string& dir) -> bool {
#ifdef _WIN32
    int result = _mkdir(dir.c_str());
#else
    int result = mkdir(dir.c_str(), 0777); // 0777 gives read, write, and execute permissions
#endif

    if (result == 0) {
        return true;
    } else {
        return false;
    }
}


auto Fast::BuildFastProject() -> void {
    std::cout << "Building the fast project." << std::endl;
    std::cout << "Project name: " << fast_project_name << std::endl;
    std::cout << "Fast Executable: " << fast_executable_name << std::endl;
    std::cout << "Fast file: " << fast_file << std::endl;
    std::cout << "Fast Modules: " << fast_modules.size() << std::endl;
    std::cout << "Fast version: " << fast_version << std::endl;
    std::cout << "Current files: " << src_fast_files.size() << std::endl;
    std::cout << "Fast compiler: " << fast_compiler_name << std::endl;

    CreateDirectory_("build");

    std::vector<std::string> object_files;
    std::cout << "Compiling object files:" << std::endl;
    for (const auto& file : src_fast_files) {
        if (GetFileExtension(file) == "hpp" || GetFileExtension(file) == "h") {
            continue;
        }
        std::string object_file = "build/" + GetFileNameWithoutExtension(file) + ".o";
        object_files.push_back(object_file);

        std::string compile_command = fast_compiler_name + " -c " + file + " -o " + object_file;
        std::cout << "\tCompileing:" << GetFileNameWithoutExtension(file) << ".o" << std::endl;
        Fast::fast_object_files.insert(object_file);

        int compile_result = std::system(compile_command.c_str());

        if (compile_result != 0) {
            std::cout << "Compilation failed for file: " << file << std::endl;
            return;
        }
    }

    std::string build_command = fast_compiler_name;

    for (const auto& object_file : object_files) {
        build_command += " " + object_file;
    }

    build_command += " -o build/" + fast_executable_name + ".exe";

    int build_result = std::system(build_command.c_str());

    if (build_result == 0) {
        std::cout << "Build successful." << std::endl;
    } else {
        std::cout << "Build failed." << std::endl;
    }
}




auto Fast::RunFast() -> void {
#ifdef _WIN32
    const std::string kExecutableExtension = ".exe";
    const std::string kPathSeparator = "\\";
#else
    const std::string kExecutableExtension = "";
    const std::string kPathSeparator = "/";
#endif

    std::cout << "Running the fast project." << std::endl;
    std::string run_command = "." + kPathSeparator + "build" + kPathSeparator + fast_executable_name + kExecutableExtension;
    std::cout << "Running: " << run_command << std::endl;
    int run_result = std::system(run_command.c_str());

    if (run_result == 0) {
    } else {
        std::cout << "Run failed." << std::endl;
    }
}
