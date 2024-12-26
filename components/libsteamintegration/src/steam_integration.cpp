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

#include <steam/steam_api.h>

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

bool steam_integration::launched_as_steam_application() {
    // First check: SteamAPI present as object was initialized
    if (mSteamInitResult != ESteamAPIInitResult::k_ESteamAPIInitResult_OK) return false;

    // Second check: rerun for liveness
    try_initialize_steamapi();
    if (mSteamInitResult != ESteamAPIInitResult::k_ESteamAPIInitResult_OK) return false;

    // Third check: okay, now we're cooking. ask Steam for things for no other reason than
    //  "cybik is playing with the API", really. This is an unofficial Launcher of Launcher,
    //  and is quite literally worlds apart from any official experience.
}

bool steam_integration::running_under_steam() {
    return (is_steam_deck() || is_steam_env() || launched_from_steam_client());
}

std::shared_ptr<steam_proton> steam_integration::proton() {
    return mProtonLayer;
}

// so that we can retry, even though it's unlikely that we should or would ever need to.
void steam_integration::try_initialize_steamapi() {
    mSteamInitResult = SteamAPI_InitEx(&mSteamInitErrMsg);
}

steam_integration::steam_integration() {
    mProtonLayer = steam_proton::getInstance();
    try_initialize_steamapi();
}

std::shared_ptr<steam_integration> steam_integration::get_steam_integration_instance() {
    if(!mInstance)
        mInstance = std::shared_ptr<steam_integration>(new steam_integration());

    return mInstance;
}
