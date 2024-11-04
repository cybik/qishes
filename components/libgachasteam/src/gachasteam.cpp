/******************************************************************
 *
 * gachasteam.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-06-29 for qgachawishes.
 *
 ******************************************************************/

#include <gachasteam.h>

#include <vlvproton.h>

#include <cstring>

#include <steam_environment.h>

std::shared_ptr<gachasteam> gachasteam::mInstance = nullptr;

bool gachasteam::is_steam_deck() {
    return SteamEnvironment::get_steam_environment_instance()->is_steam_deck();
}

bool gachasteam::is_steam_env() {
    return SteamEnvironment::get_steam_environment_instance()->is_steam_env();
}

bool gachasteam::launched_from_steam_client() {
    return SteamEnvironment::get_steam_environment_instance()->launched_from_steam_client();
}

bool gachasteam::running_under_steam() {
    return (is_steam_deck() || is_steam_env() || launched_from_steam_client());
}

std::shared_ptr<gachasteam> gachasteam::get_gachasteam_instance() {
    if(!mInstance)
        mInstance = std::shared_ptr<gachasteam>(new gachasteam());
    return mInstance;
}
