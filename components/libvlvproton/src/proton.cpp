/******************************************************************
 *
 * proton.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-11-29, for qgachawishes.
 *
 ******************************************************************/

#include <proton.h>

proton::proton(std::filesystem::path dir) {
    mIsValidConfirmed = is_regular_file((dir / "proton"));
    if (mIsValidConfirmed) {
        mResolvedProtonExec = (dir / "proton");
    }
}

std::string proton::exec() {
    return mResolvedProtonExec;
}

std::string proton::dir() {
    return mResolvedProtonExec.parent_path();
}
