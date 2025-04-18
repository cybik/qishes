/******************************************************************
 *
 * HonkaiImpact3rd.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-17, for qgachawishes.
 *
 ******************************************************************/

#include "HonkaiImpact3rd.h"

std::string HonkaiImpact3rd::getExecutableName() {
    return "BH3.exe";
}
GameInfo::ExeType HonkaiImpact3rd::getGameType() {
    return GameInfo::ExeType::Honkai3rd;
}
std::string HonkaiImpact3rd::getLabel() {
    return "Honkai: Impact 3rd";
}
Workaround::Handler HonkaiImpact3rd::getWorkaround() {
    return Workaround::Handler::Jadeite;
}

std::filesystem::path HonkaiImpact3rd::getExecutablePath(std::filesystem::path searchRoot) {
    abort();
}