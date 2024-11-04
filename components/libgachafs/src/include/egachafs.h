/******************************************************************
 *
 * egachafs.h
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-08-25 for libgachafs, part of qgachawishes
 *
 ******************************************************************/

#pragma once

#include <string>

class EGachaFS_Exception: public std::runtime_error {
public:
    explicit EGachaFS_Exception(const std::string& str) : std::runtime_error(str) {};
};