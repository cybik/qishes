/******************************************************************
 *
 * vlvproton.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-11-04 for qgachawishes.
 *
 ******************************************************************/

#include "vlvproton.h"

#include <filesystem>
#include <iostream>
#include <ranges>

#include <termcolor/termcolor.hpp>

std::shared_ptr<vlvproton> vlvproton::mInstance = nullptr;

std::shared_ptr<vlvproton> vlvproton::getInstance(const std::filesystem::path& base_dir) {
    if (!mInstance)
        mInstance = std::shared_ptr<vlvproton>(new vlvproton(base_dir));
    return mInstance;
}
std::shared_ptr<vlvproton> vlvproton::getInstance() {
    if (!mInstance)
        abort(); // literally shouldn't get here. failure by design.
    return mInstance;
}

std::vector<std::string> vlvproton::get_available_protons() {
    auto view = std::views::keys(mProtons);
    return std::vector<std::string>{view.begin(), view.end() };
}

std::shared_ptr<proton> vlvproton::get_selected_proton() {
    return mProtons.at(mSelectedProton);
}

void vlvproton::select(std::string key) {
    mSelectedProton = key;
}

vlvproton::vlvproton(std::filesystem::path base_dir) {
    m_baseDir = base_dir.empty()
        ? std::filesystem::path(std::getenv("HOME")) / ".steam/root" // this is a backup edge case
        : base_dir;
    for (const auto& rootdir : {
        (m_baseDir / "compatibilitytools.d"), (m_baseDir / "steamapps" / "common")
    } ) {
        identify( std::filesystem::directory_iterator(rootdir));
    }
    for (auto [proton_key, proton_value]: mProtons) {
        std::cout
            << termcolor::bright_magenta
                << "Proton identified: " << proton_key
            << termcolor::reset
                << " at "
            << termcolor::bright_blue
                << proton_value->dir()
            << termcolor::reset
        << std::endl;
    }
}

void vlvproton::identify(std::filesystem::directory_iterator path) {
    // skip symlinks and determine if proton runtime is present
    for (const auto& dir : std::filesystem::directory_iterator(path))
        if (dir.is_directory() && !is_symlink(dir) && is_regular_file(dir.path() / "proton") )
            mProtons.emplace(dir.path().filename(), std::make_shared<proton>(dir));
}
