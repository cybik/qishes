/******************************************************************
 *
 * steamrt.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 26-02-16, for qgachawishes.
 *
 ******************************************************************/

#include <steamrt.h>

steamrt::steamrt(std::string name, std::filesystem::path dir) {
    mName = name;
    mIsValidConfirmed = is_regular_file((dir / "_v2-entry-point"));
    if (mIsValidConfirmed) {
        mResolvedSteamRTExec = (dir / "_v2-entry-point");
    }
}

steamrt::SteamRT_Type steamrt::type() {
    return (mIsValidConfirmed?SteamRT:None);
}

std::string steamrt::name() {
    return mName;
}

std::string steamrt::exec() {
    return mResolvedSteamRTExec;
}

std::string steamrt::dir() {
    return mResolvedSteamRTExec.parent_path();
}
