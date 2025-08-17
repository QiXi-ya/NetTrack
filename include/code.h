#pragma once
#include "Main.h"
#include <string>
#include <vector>

class StringVectorConverter {
public:
    static std::string join(const std::vector<std::string>& vec, char delimiter);
    static std::vector<std::string> split(const std::string& str, char delimiter);
    StringVectorConverter() = delete;
    StringVectorConverter(const StringVectorConverter&) = delete;
    void operator=(const StringVectorConverter&) = delete;
};