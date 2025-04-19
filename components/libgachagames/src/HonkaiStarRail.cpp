/******************************************************************
 *
 * HonkaiStarRail.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-17, for qgachawishes.
 *
 ******************************************************************/

#include "HonkaiStarRail.h"
//GameInfo("StarRail.exe", "Honkai: Star Rail", GameInfo::HonkaiSR, Workaround::Handler::Jadeite)
std::string HonkaiStarRail::getExecutableName() {
    return "StarRail.exe";
}
GameInfo::ExeType HonkaiStarRail::getGameType() {
    return GameInfo::ExeType::HonkaiSR;
}
std::string HonkaiStarRail::getLabel() {
    return "Honkai: Star Rail";
}

Workaround::Handler HonkaiStarRail::getWorkaround() {
    return Workaround::Handler::Jadeite;
}
