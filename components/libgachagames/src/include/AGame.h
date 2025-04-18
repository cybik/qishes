/******************************************************************
 *
 * AGame.h
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-17, for qgachawishes.
 *
 ******************************************************************/


#pragma once
#include <filesystem>
#include <list>

#include "GameInfo.h"
#include <memory>
#include <workaround.h>

class AGame {
public:
    virtual ~AGame() = default;

    static std::shared_ptr<std::list<std::shared_ptr<AGame>>> getSupportedGames();
    virtual std::string getExecutableName() = 0;
    virtual std::string getLabel() = 0;
    virtual GameInfo::ExeType getGameType() = 0;
    virtual Workaround::Handler getWorkaround() = 0;
    virtual std::filesystem::path getExecutablePath(std::filesystem::path searchRoot) = 0;
    virtual void prepareEnvironment() {}

    GameInfo getGameInfo();
    // todo: "seek true exe from path" for Wuwa
    // todo: "get background for launcher ay"
};
