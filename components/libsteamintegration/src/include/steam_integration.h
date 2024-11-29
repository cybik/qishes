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

#include "steam_proton.h"

class steam_integration {
public:
    static std::shared_ptr<steam_integration>   get_steam_integration_instance();
    bool                                        is_steam_deck();
    bool                                        is_steam_env();
    bool                                        launched_from_steam_client();
    bool                                        running_under_steam();
    std::shared_ptr<steam_proton>               proton();
protected:
private:
    steam_integration();

    static std::shared_ptr<steam_integration> mInstance;
    std::shared_ptr<steam_proton>                                       mProtonLayer;
};
