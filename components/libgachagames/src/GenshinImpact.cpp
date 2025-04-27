/******************************************************************
 *
 * GenshinImpact.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-17, for qgachawishes.
 *
 ******************************************************************/

#include "GenshinImpact.h"
#include <steam_integration.h>

#include <util/log.h>
std::string GenshinImpact::getExecutableName() {
    return "GenshinImpact.exe";
}

GameInfo::ExeType GenshinImpact::getGameType() {
    return GameInfo::ExeType::Genshin;
}

std::string GenshinImpact::getLabel() {
    return "Genshin Impact";
}

Workaround::Handler GenshinImpact::getWorkaround() {
    return Workaround::Handler::None;
}


void GenshinImpact::prepareEnvironment() {
    // Genshin Impact shader mis-generation workaround
    Log::get_logger()->info("Correcting decimal separator for Genshin shader tech.");
    steam_integration::get_steam_integration_instance()->proton()->try_run(
        "reg",
        Workaround::Handler::None,
        {
            "add", "\"HKCU\\Control Panel\\International\"",
            "/v", "sDecimal",   // Value name
            "/t", "REG_SZ",     // Type string
            "/d", "\".\"",      // Data proper
            "/f"                   // Force write
        },
        {},
        ""
    );
}

void GenshinImpact::prepare() {
    AHoyoGame::prefetchData(HoyoConstants::loc_hk4e);
}

std::list<std::string> GenshinImpact::processArguments(LaunchOptions lo) {
    switch (lo) {
        case LaunchOptions::CloudOverride: {
            return std::list<std::string> {
                "-platform_type",
                "CLOUD_THIRD_PARTY_PC",
            };
        }
    }
}
