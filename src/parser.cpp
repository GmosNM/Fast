#include "parser.hpp"

std::string Parser::readFile(const std::string& file) {
    std::ifstream inputFile(file);
    if (!inputFile) {
        std::cerr << "Error opening file: " << file << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    return buffer.str();
}

void Parser::parse(const std::string& file) {
    std::string fileContent = readFile(file);
    if (fileContent.empty()) {
        std::cerr << "Failed to read file: " << file << std::endl;
        return;
    }

    std::string executableName;
    std::string compilerName;

    std::istringstream iss(fileContent);
    std::string line;
    while (std::getline(iss, line)) {
        if (line.find("EXECUTABLE:") != std::string::npos) {
            executableName = line.substr(line.find(":") + 1);
        } else if (line.find("COMPILER:") != std::string::npos) {
            compilerName = line.substr(line.find(":") + 1);
        }
    }

    size_t start = compilerName.find_first_not_of(" \t");
    if (start != std::string::npos) {
        compilerName = compilerName.substr(start);
    }

    size_t start2 = executableName.find_first_not_of(" \t");
    if (start2 != std::string::npos) {
        executableName = executableName.substr(start2);
    }

    size_t end = compilerName.find_last_not_of(" \t");
    if (end != std::string::npos) {
        executableName = executableName.substr(0, end);
    }

    fast.setExecutableName(executableName);
    fast.setCompiler(compilerName);
    CompilerName_ = compilerName + " ";
    ExecutableName_ = executableName;
}



