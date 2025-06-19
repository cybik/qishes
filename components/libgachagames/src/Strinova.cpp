/******************************************************************
 *
 * Strinova.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-17, for qgachawishes.
 *
 ******************************************************************/

#include "Strinova.h"
#include <gachafs.h>

#include <steam_integration.h>

#include <util/log.h>

std::string Strinova::getExecutableName() {
    return "Strinova.exe";
}

GameInfo::ExeType Strinova::getGameType() {
    return GameInfo::ExeType::Strinova;
}

std::string Strinova::getLabel() {
    return "Strinova";
}

Workaround::Handler Strinova::getWorkaround() {
    return Workaround::Handler::None;
}

std::map<std::string, std::string> Strinova::getEnvironment() {
    return {
        //{ "WINEDLLOVERRIDES", "KRSDKExternal.exe=d;winegstreamer=;mfplat=d" },
        //{ "PROTON_DISABLE_NVAPI","1" },
        //{ "SteamGameId", GAME_ID },
        //{ "WINEDLLOVERRIDES", "KRSDKExternal.exe=d" },
        //{ "RADV_PERFTEST", "rt" }
    };
}

std::filesystem::path Strinova::getExecutablePath() {
    for (auto file : *gachafs::getFsFiles(
        "**/Strinova.exe",
        //"**/Strinova-Win64-Shipping.exe",
        QString::fromStdString(AGame::getExecutablePath().parent_path().c_str()),
        //QString::fromStdString(AGame::getExecutablePath().parent_path().parent_path().c_str()),
        false
        )
    ) { // will return ONE element.
        Log::get_logger()->critical(file.c_str());
        return file;
    }
    // should not reach this, actually.
    abort();
}

std::list<std::string> Strinova::getArguments() {
    return {};
}

std::string Strinova::getGameShorthand() {
    return "nova";
}

void Strinova::prepareEnvironment() {
    // Genshin Impact shader mis-generation workaround
    Log::get_logger()->info("Bad reg override for Strinova on Linux.");
    for (auto single_pair : {std::pair("Auto", "0"), std::pair("Debugger", "")}) {
        Log::get_logger()->warning(QString("Reg Override ") + single_pair.first);
        steam_integration::get_steam_integration_instance()->proton()->try_run(
            "reg",
            Workaround::Handler::None,
            { // add "HKLM\\Software\\Microsoft\\Windows_NT\\CurrentVersion\\AeDebug" /v "Auto" /t "REG_SZ" /d "0" /f
                "add", "HKLM\\Software\\Microsoft\\Windows NT\\CurrentVersion\\AeDebug",
                "/v", single_pair.first,   // Value name
                "/t", "REG_SZ",     // Type string
                "/d", single_pair.second,      // Data proper
                "/f"                   // Force write
            },
            {},
            ""
        );
    }
}

std::list<int> Strinova::getSteamIdentifiers() {
    return {1282270, 1851210};
}


void Strinova::prepare() {
}