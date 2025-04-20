/******************************************************************
 *
 * Launcher.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-17, for qgachawishes.
 *
 ******************************************************************/

#include "Launcher.h"
#include <steam_integration.h>

#include <util/log.h>
std::string Launcher::getExecutableName() {
    return "launcher.exe";
}

GameInfo::ExeType Launcher::getGameType() {
    return GameInfo::ExeType::Launcher;
}

std::string Launcher::getLabel() {
    return "Launcher";
}

Workaround::Handler Launcher::getWorkaround() {
    return Workaround::Handler::None;
}
