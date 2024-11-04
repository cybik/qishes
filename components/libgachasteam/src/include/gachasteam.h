/******************************************************************
 *
 * gachasteam.h
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-06-29 for qgachawishes.
 *
 ******************************************************************/
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
};
