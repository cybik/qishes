//
// Created by cybik on 24-08-25.
//

#pragma once

#include <string>

class EGachaFS_Exception: public std::runtime_error {
public:
    explicit EGachaFS_Exception(const std::string& str) : std::runtime_error(str) {};
};