/******************************************************************
 *
 * ZenlessZoneZero.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-17, for qgachawishes.
 *
 ******************************************************************/

#include "ZenlessZoneZero.h"


std::string ZenlessZoneZero::getExecutableName() {
    return "ZenlessZoneZero.exe";
}
GameInfo::ExeType ZenlessZoneZero::getGameType() {
    return GameInfo::ExeType::Nap;
}
std::string ZenlessZoneZero::getLabel() {
    return "Zenless Zone Zero";
}

Workaround::Handler ZenlessZoneZero::getWorkaround() {
    return Workaround::Handler::None;
}
