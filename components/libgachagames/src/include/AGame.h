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
#include <map>

#include "GameInfo.h"
#include <memory>
#include <workaround.h>

class AGame {
public:
    virtual ~AGame() = default;
    typedef enum {
        CloudOverride
    } LaunchOptions;

    static std::shared_ptr<std::list<std::shared_ptr<AGame>>> getSupportedGames();
    virtual std::string getExecutableName() = 0;
    virtual std::string getLabel() = 0;
    virtual GameInfo::ExeType getGameType() = 0;
    virtual Workaround::Handler getWorkaround() = 0;
    //virtual void prefetchBackground(HoyoConstants::HoyoServerKey) = 0;
    virtual void prepare() = 0;
    virtual std::filesystem::path getExecutablePath();
    virtual std::list<std::string> getArguments();
    virtual std::map<std::string, std::string> getEnvironment();
    virtual void setExecutablePath(std::filesystem::path executablePath);

    virtual std::string getBackground() = 0;

    // inline default
    virtual void prepareEnvironment() {}

    static std::shared_ptr<AGame> identify(std::string checkName);

    virtual std::list<std::string> processArguments(LaunchOptions);

    virtual std::string getGameShorthand();

    GameInfo getGameInfo();
    // todo: "seek true exe from path" for Wuwa
    // todo: "get background for launcher ay"
    std::filesystem::path executablePath;
};
