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

#include <steam/steam_api.h>

class steam_integration {
public:
    static std::shared_ptr<steam_integration>   get_steam_integration_instance();
    bool                                        is_steam_deck();
    bool                                        is_steam_env();
    bool                                        launched_from_steam_client();
    bool                                        launched_as_steam_application();
    bool                                        running_under_steam();
    std::shared_ptr<steam_proton>               proton();
    void                                        set_the_dumb_achievement();
protected:
private:
    /*CTOR*/                                    steam_integration();
    void                                        try_initialize_steamapi();

    static std::shared_ptr<steam_integration>   mInstance;
    std::shared_ptr<steam_proton>               mProtonLayer;

    ESteamAPIInitResult                         mSteamInitResult;
    SteamErrMsg                                 mSteamInitErrMsg;
    bool                                        mSteamSession;
};
