/******************************************************************
 *
 * steam_integration.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-06-29 for qgachawishes.
 *
 ******************************************************************/

#include <steam_integration.h>

#include <steam_proton.h>

#include <cstring>

#include <steam_environment.h>

#include "steam_proton.h"

std::shared_ptr<steam_integration> steam_integration::mInstance = nullptr;

bool steam_integration::is_steam_deck() {
    return SteamEnvironment::get_steam_environment_instance()->is_steam_deck();
}

bool steam_integration::is_steam_env() {
    return SteamEnvironment::get_steam_environment_instance()->is_steam_env();
}

bool steam_integration::launched_from_steam_client() {
    return SteamEnvironment::get_steam_environment_instance()->launched_from_steam_client();
}

bool steam_integration::running_under_steam() {
    return (is_steam_deck() || is_steam_env() || launched_from_steam_client());
}

std::shared_ptr<steam_proton> steam_integration::proton() {
    return mProtonLayer;
}

steam_integration::steam_integration() {
    mProtonLayer = steam_proton::getInstance();
}

std::shared_ptr<steam_integration> steam_integration::get_steam_integration_instance() {
    if(!mInstance)
        mInstance = std::shared_ptr<steam_integration>(new steam_integration());

    return mInstance;
}
