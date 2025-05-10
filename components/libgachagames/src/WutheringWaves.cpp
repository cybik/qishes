/******************************************************************
 *
 * WutheringWaves.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-17, for qgachawishes.
 *
 ******************************************************************/

#include "WutheringWaves.h"
#include <util/log.h>

#include <gachafs.h>

// Known Wuthering Waves Steam GameID
const std::string GAME_ID = "3513350";

std::string WutheringWaves::getExecutableName() {
    return "Wuthering Waves.exe";
}

GameInfo::ExeType WutheringWaves::getGameType() {
    return GameInfo::ExeType::WutheringWaves;
}

std::string WutheringWaves::getLabel() {
    return "Wuthering Waves";
}

Workaround::Handler WutheringWaves::getWorkaround() {
    return Workaround::Handler::Jadeite;
}

void WutheringWaves::registerEffectors() {

}


std::map<std::string, std::string> WutheringWaves::getEnvironment() {
    return {
        //{ "WINEDLLOVERRIDES", "KRSDKExternal.exe=d;winegstreamer=;mfplat=d" },
        //{ "PROTON_DISABLE_NVAPI","1" },
        //{ "SteamGameId", GAME_ID },
        { "WINEDLLOVERRIDES", "KRSDKExternal.exe=d" },
        { "RADV_PERFTEST", "rt" }
    };
}

// Proton arg: dx11 for Proton9. 10 is needed for both nVidia and AMD to run DX12 / VKD3D
std::list<std::string> WutheringWaves::getArguments() {
    return {"--", "-dx12"}; // -- after Jadeite, then dx11
}

// export WINEDLLOVERRIDES="KRSDKExternal.exe=d"

std::filesystem::path WutheringWaves::getExecutablePath() {
    Log::get_logger()->critical("Honq");
    for (auto file : *gachafs::getFsFiles(
        "**/Client-Win64-Shipping.exe",
        QString::fromStdString(AGame::getExecutablePath().parent_path().c_str()),
        false
        )
    ) { // will return ONE element.
        Log::get_logger()->critical(file.c_str());
        return file;
    }
    // should not reach this, actually.
    abort();
}

std::string WutheringWaves::getGameShorthand() {
    return "wuwa";
}

