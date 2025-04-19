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

std::string WutheringWaves::getExecutableName() {
    return "WutheringWaves.exe";
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

std::filesystem::path WutheringWaves::getExecutablePath() {
    abort();
}
