/******************************************************************
 *
 * steam_environment.h
 *
 * Created by cybik on 24-11-04 for qgachawishes.
 *
 ******************************************************************/

#pragma once
#include <memory>

class SteamEnvironment {
public:
    static std::shared_ptr<SteamEnvironment>    get_steam_environment_instance();
    bool                                        is_steam_deck();
    bool                                        is_steam_env();
    bool                                        launched_from_steam_client();
protected:
private:
    SteamEnvironment() = default;
    static std::shared_ptr<SteamEnvironment> mInstance;
    bool check_env_var_against_value(const char* env_var, const char* expected_value);

};
