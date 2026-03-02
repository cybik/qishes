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

std::shared_ptr<vlvproton> vlvproton::getInstance(std::list<std::filesystem::path> base_dirs) {
    if (!mInstance)
        mInstance = std::shared_ptr<vlvproton>(new vlvproton(base_dirs));
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

std::vector<std::string> vlvproton::get_available_steam_runtimes() {
    auto view = std::views::keys(mSteamRTs);
    return std::vector<std::string>{view.begin(), view.end() };
}

std::shared_ptr<proton> vlvproton::get_selected_proton() {
    return mProtons[mSelectedProton];
}

std::shared_ptr<steamrt> vlvproton::get_selected_steamrt() {
    return mSteamRTs[mSelectedSteamRT];
}

void vlvproton::select(const std::string& key) {
    mSelectedProton = key;
}

void vlvproton::selectSteamRt(const std::string& key) {
    mSelectedSteamRT = key;
}

void vlvproton::identify_proton_installs() {
    for (const auto& [proton_key, proton_value]: mProtons) {
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

void vlvproton::identify_steamrt_installs() {
    for (const auto& [steamrt_key, steamrt_value]: mSteamRTs) {
        std::cout
            << termcolor::bright_magenta
                << "Steam Runtime identified: " << steamrt_key
            << termcolor::reset
                << " at "
            << termcolor::bright_blue
                << steamrt_value->dir()
            << termcolor::reset
        << std::endl;
    }
}


vlvproton::vlvproton(std::list<std::filesystem::path> base_dirs) {
    if (base_dirs.empty()) {
        abort(); // handle this at some point
    }
    for (auto path: base_dirs) {
        // Identifying Proton
        for (const auto& rootdir : {
            (path / "compatibilitytools.d"),
            (path / "steamapps" / "common")
        } ) {
            try {
                identify_proton( std::filesystem::directory_iterator(rootdir));
                identify_steamrt( std::filesystem::directory_iterator(rootdir));
            } catch (std::filesystem::filesystem_error const& ex) { /* just skip lol */ }
        }
    }
    //identify_installs();
}

void vlvproton::identify_steamrt(const std::filesystem::directory_iterator& path) {
    // skip symlinks and determine if proton runtime is present
    mSteamRTs.emplace("None", std::make_shared<steamrt>("None", ""));
    for (const auto& dir : std::filesystem::directory_iterator(path))
        if (dir.is_directory() && !is_symlink(dir) && is_regular_file(dir.path() / "_v2-entry-point") )
            mSteamRTs.emplace(dir.path().filename(), std::make_shared<steamrt>(dir.path().filename(), dir));
}


void vlvproton::identify_proton(const std::filesystem::directory_iterator& path) {
    // skip symlinks and determine if proton runtime is present
    for (const auto& dir : std::filesystem::directory_iterator(path))
        if (dir.is_directory() && !is_symlink(dir) && is_regular_file(dir.path() / "proton") )
            mProtons.emplace(dir.path().filename(), std::make_shared<proton>(dir));
}
