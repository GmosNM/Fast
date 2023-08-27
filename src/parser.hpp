#pragma once
#include "fast.hpp"

class Parser {
public:
    Parser() = default;

    ~Parser() {}

    std::string readFile(const std::string& file);
    void parse(const std::string& file);

    std::string getCompilerName(){
        return CompilerName_;
    }

    std::string getExecutableName(){
        return ExecutableName_;
    }

private:
    Fast fast;
    std::string fastFile;
    std::string CompilerName_;
    std::string ExecutableName_;
};
