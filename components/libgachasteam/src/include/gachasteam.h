//
// Created by cybik on 24-06-29.
//

#pragma once
#include <memory>

class gachasteam {
public:
    static std::shared_ptr<gachasteam> get_gachasteam_instance();
    bool                               is_steam_deck();
    bool                               is_steam_env();
    bool                               launched_from_steam_client();
    bool                               running_under_steam();
protected:
private:
    gachasteam() = default;

    static std::shared_ptr<gachasteam> mInstance;

    bool check_env_var_against_value(const char* env_var, const char* expected_value);
};
