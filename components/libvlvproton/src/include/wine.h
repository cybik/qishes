//
// Created by cybik on 24-12-23.
//

#pragma once

#include <filesystem>
#include <string>

class wine {
public:
    static std::filesystem::path resolve_executable(const std::string & string);
    static std::string resolve_executable_path(const std::string & string);
};
