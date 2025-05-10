/******************************************************************
 *
 * AHoyoGame.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-19, for qgachawishes.
 *
 ******************************************************************/

#include "AHoyoGame.h"

#include <iostream>

void AHoyoGame::prefetchData(HoyoConstants::HoyoServerKey key) {
    mBackground = HoyoMetadata::get_instance()->getBackground(key);
}

std::map<std::string, std::string> AHoyoGame::getEnvironment() {
    return {
        { "RADV_PERFTEST", "rt" }
    };
}

std::string AHoyoGame::getBackground() {
    return mBackground;
}
