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

std::string AHoyoGame::getBackground() {
    return mBackground;
}
