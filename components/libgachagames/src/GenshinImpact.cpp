/******************************************************************
 *
 * genshin_impact.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-17, for qgachawishes.
 *
 ******************************************************************/

#include "include/GenshinImpact.h"

std::string GenshinImpact::getExecutableName() {
    return "GenshinImpact.exe";
}

GameInfo::ExeType GenshinImpact::getGameType() {
    return GameInfo::ExeType::Genshin;
}
std::string GenshinImpact::getLabel() {
    return "Genshin Impact";
}
Workaround::Handler GenshinImpact::getWorkaround() {
    return Workaround::Handler::None;
}

std::filesystem::path GenshinImpact::getExecutablePath(std::filesystem::path searchRoot) {
    abort();
}
