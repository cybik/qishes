/******************************************************************
 *
 * Strinova.h
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-17, for qgachawishes.
 *
 ******************************************************************/


#pragma once

#include "AGame.h"

class Strinova : public AGame {
public:
    std::string getExecutableName() override;
    GameInfo::ExeType getGameType() override;
    std::string getLabel() override;
    Workaround::Handler getWorkaround() override;
    std::filesystem::path getExecutablePath() override;
    std::map<std::string, std::string> getEnvironment() override;

    std::list<std::string> getArguments() override;
    void prepareEnvironment() override;
    void prepare() override;
    std::string getBackground() override { return ""; }
    std::string getGameShorthand() override;

    std::list<int> getSteamIdentifiers() override;
};
