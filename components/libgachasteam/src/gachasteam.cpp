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

std::shared_ptr<gachasteam> gachasteam::mInstance = nullptr;

bool gachasteam::is_steam_deck() {
    return check_env_var_against_value("SteamDeck", "1");
}

bool gachasteam::is_steam_env() {
    return check_env_var_against_value("SteamEnv", "1");
}

bool gachasteam::launched_from_steam_client() {
    return check_env_var_against_value("SteamClientLaunch", "1");
}

bool gachasteam::running_under_steam() {
    return (is_steam_deck() || is_steam_env() || launched_from_steam_client());
}

bool gachasteam::check_env_var_against_value(const char* env_var, const char* expected_value) {
    const char* env_var_value = std::getenv(env_var);
    return ( env_var_value && strcmp(env_var_value, expected_value) == 0 );
}

std::shared_ptr<gachasteam> gachasteam::get_gachasteam_instance() {
    if(!mInstance) mInstance = std::shared_ptr<gachasteam>(new gachasteam());
    return mInstance;
}
