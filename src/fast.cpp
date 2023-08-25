#include "fast.hpp"


void Fast::init() {
    std::string filename = ".fast";
    std::fstream file(filename, std::ios::in);

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

auto Fast::WalkIntoDir(std::string dir) -> std::vector<std::string> {
    std::vector<std::string> files;
    for (auto& file : std::filesystem::directory_iterator(dir)) {
        files.push_back(file.path().string());
    }
    return files;
}

auto Fast::CleanDirVectorPath(std::string dir) -> std::vector<std::string> {
    if (!std::filesystem::exists(dir)) {
        std::cout << "Creating folder: " << dir << std::endl;
        std::filesystem::create_directory(dir);
    }

    std::vector<std::string> files = this->WalkIntoDir(dir);
    for (auto& file : files) {
        size_t pos = file.find("./");
        while (pos != std::string::npos) {
            file.erase(pos, 2);
            pos = file.find("./");
        }
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
    std::fstream _fast_file(Fast::fast_file, std::ios::out);
    _fast_file << "VERSION: " << Fast::fast_version << std::endl;
    _fast_file << "PROJECT: " << Fast::fast_project_name << std::endl;
    _fast_file << "\n\nFILES:" << std::endl;
    for (const auto& file : this->src_fast_files) {
        _fast_file << "   " << file << std::endl;
    }
    _fast_file.close();
}

auto Fast::GetDirName() -> std::string {
    return std::filesystem::path(std::filesystem::current_path()).filename().string();
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

auto Fast::BuildFastProject() -> void {
    std::cout << "Building the fast project." << std::endl;
    std::cout << "Project name: " << fast_project_name << std::endl;
    std::cout << "Fast file: " << fast_file << std::endl;
    std::cout << "Fast version: " << fast_version << std::endl;
    std::cout << "Current files: " << src_fast_files.size() << std::endl;
    std::cout << "Fast compiler: " << fast_compiler_name << std::endl;

    std::filesystem::create_directory("build");

    std::vector<std::string> object_files;

    for (const auto& file : src_fast_files) {
        if (GetFileExtension(file) == "hpp" || GetFileExtension(file) == "h") {
            continue;
        }
        std::string object_file = "build/" + GetFileNameWithoutExtension(file) + ".o";
        object_files.push_back(object_file);

        std::string compile_command = "" + fast_compiler_name + "-c " + file + " -o " + object_file;
        std::cout << "Compile command: " << compile_command << std::endl;

        int compile_result = std::system(compile_command.c_str());

        if (compile_result != 0) {
            std::cout << "Compilation failed for file: " << file << std::endl;
            return;
        }
    }

    std::string build_command = fast_compiler_name;

    for (const auto& object_file : object_files) {
        build_command += object_file + " ";
    }

    build_command += "-o build/" + fast_project_name + ".exe";
    std::cout << "Build command: " << build_command << std::endl;

    int build_result = std::system(build_command.c_str());

    if (build_result == 0) {
        std::cout << "Build successful." << std::endl;
    } else {
        std::cout << "Build failed." << std::endl;
    }
}

