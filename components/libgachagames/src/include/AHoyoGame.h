/******************************************************************
 *
 * AHoyoGame.h
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-19, for qgachawishes.
 *
 ******************************************************************/

#pragma once

#include "AGame.h"

#include "util/HoyoMetadata.h"

class AHoyoGame : public AGame {
public:
    void prefetchData(HoyoConstants::HoyoServerKey);

    std::map<std::string, std::string> getEnvironment() override;

    std::string getBackground() override;
private:
    std::string  mBackground;
};
