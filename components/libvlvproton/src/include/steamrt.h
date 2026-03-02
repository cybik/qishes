/******************************************************************
 *
 * steamrt.h
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 26-02-16, for qgachawishes.
 *
 ******************************************************************/

#pragma once

#include <filesystem>

// TODO: get this properly done
class steamrt {
public:
    typedef enum {
        None = 0,
        SteamRT
    } SteamRT_Type;
    steamrt(std::string name, std::filesystem::path);
    std::string name();
    std::string dir();
    std::string exec();
    SteamRT_Type type();
protected:
private:
    std::string mName;
    std::filesystem::path mRootdir;
    bool mIsValidConfirmed = false;
    std::filesystem::path mResolvedSteamRTExec;
};
