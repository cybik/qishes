//
// Created by cybik on 24-06-29.
//

#include <gachasteam.h>

#include <vlvproton.h>

std::shared_ptr<gachasteam> gachasteam::mInstance = nullptr;

bool gachasteam::running_under_steam() {
    const char* steam_envs[2] = {
        std::getenv("SteamDeck"),
        std::getenv("SteamEnv"),
        //std::getenv("SteamAppId"),
        //std::getenv("SteamClientLaunch"),
        //std::getenv("STEAM_COMPAT_DATA_PATH"),
        //std::getenv("STEAM_COMPAT_TOOL_PATHS"),
    };
    return (steam_envs[0] || steam_envs[1]);
}

std::shared_ptr<gachasteam> gachasteam::get_gachasteam_instance() {
    if(!mInstance) mInstance = std::shared_ptr<gachasteam>(new gachasteam());
    return mInstance;
}
