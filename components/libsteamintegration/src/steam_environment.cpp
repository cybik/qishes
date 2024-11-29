/******************************************************************
 *
 * steam_environment.cpp
 *
 * Created by cybik on 24-11-04 for qgachawishes.
 *
 ******************************************************************/

#include <steam_environment.h>

#include <cstring>
#include <filesystem>

std::shared_ptr<SteamEnvironment> SteamEnvironment::mInstance = nullptr;
std::shared_ptr<SteamEnvironment> SteamEnvironment::get_steam_environment_instance() {
    if (!mInstance)
        mInstance = std::shared_ptr<SteamEnvironment>(new SteamEnvironment());
    return mInstance;
}

bool SteamEnvironment::is_steam_deck() {
    return check_env_var_against_value("SteamDeck", "1");
}

bool SteamEnvironment::is_steam_env() {
    return check_env_var_against_value("SteamEnv", "1");
}

bool SteamEnvironment::launched_from_steam_client() {
    return check_env_var_against_value("SteamClientLaunch", "1");
}

std::filesystem::path SteamEnvironment::getSteamBaseFolder() {
    if (const char* base_folder = std::getenv("STEAM_BASE_FOLDER"); base_folder != nullptr) {
        return  { base_folder };
    }
    // Default. Proooobably bad.
    return { std::filesystem::path(std::getenv("HOME")) / ".steam/root" };
}

bool SteamEnvironment::check_env_var_against_value(const char* env_var, const char* expected_value) {
    const char* env_var_value = std::getenv(env_var);
    return ( env_var_value && strcmp(env_var_value, expected_value) == 0 );
}