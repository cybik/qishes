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
#include <GenshinImpact.h>
#include <HonkaiImpact3rd.h>
#include <HonkaiStarRail.h>
#include <ZenlessZoneZero.h>

std::shared_ptr<std::list<std::shared_ptr<AGame>>> AGame::getSupportedGames() {
    std::shared_ptr<std::list<std::shared_ptr<AGame>>> ret = std::make_shared<std::list<std::shared_ptr<AGame>>>();
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
