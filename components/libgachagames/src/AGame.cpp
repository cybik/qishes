/******************************************************************
 *
 * a_game.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-17, for qgachawishes.
 *
 ******************************************************************/

#include "include/AGame.h"

// All supported games for now.
// TODO: a class registry
#include <WutheringWaves.h>
#include <Launcher.h>
#include <GenshinImpact.h>
#include <HonkaiImpact3rd.h>
#include <HonkaiStarRail.h>
#include <ZenlessZoneZero.h>

std::shared_ptr<std::list<std::shared_ptr<AGame>>> AGame::getSupportedGames() {
    std::shared_ptr<std::list<std::shared_ptr<AGame>>> ret = std::make_shared<std::list<std::shared_ptr<AGame>>>();
    ret->push_back(std::move(std::make_shared<Launcher>()));
    ret->push_back(std::move(std::make_shared<WutheringWaves>()));
    ret->push_back(std::move(std::make_shared<GenshinImpact>()));
    ret->push_back(std::move(std::make_shared<HonkaiImpact3rd>()));
    ret->push_back(std::move(std::make_shared<HonkaiStarRail>()));
    ret->push_back(std::move(std::make_shared<ZenlessZoneZero>()));
    return ret;
}

GameInfo AGame::getGameInfo() {
    return GameInfo(getExecutableName(), getLabel(), getGameType(), getWorkaround());
}

void AGame::setExecutablePath(std::filesystem::path executablePath) {
    this->executablePath = executablePath;
}

std::shared_ptr<AGame> AGame::identify(std::string checkName) {
    std::filesystem::path check_path = checkName;
    for (std::shared_ptr<AGame> game: *getSupportedGames()) {
        if (game->getExecutableName().compare(check_path.filename()) == 0) {
            // Identified and isn't a launcher.
            if (game->getGameType() != GameInfo::ExeType::Launcher) {
                game->setExecutablePath(check_path);
                game->prepare();
                return game;
            }
        }
    }
    return nullptr;
}

// Determine true exe downstream, if necessary.
std::filesystem::path AGame::getExecutablePath() {
    return this->executablePath;
}

std::list<std::string> AGame::getArguments() {
    return {};
}

std::map<std::string, std::string> AGame::getEnvironment() {
    return { };
}

