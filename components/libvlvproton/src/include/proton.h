/******************************************************************
 *
 * proton.h
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-11-29, for qgachawishes.
 *
 ******************************************************************/

#pragma once

#include <filesystem>

class proton {
public:
    proton(std::filesystem::path);
    std::string dir();
    std::string exec();
protected:
private:
    std::filesystem::path mRootdir;
    bool mIsValidConfirmed = false;
    std::filesystem::path mResolvedProtonExec;
};