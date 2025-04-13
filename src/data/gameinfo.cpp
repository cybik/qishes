/******************************************************************
 *
 * gameinfo.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-10, for qgachawishes.
 *
 ******************************************************************/

#include "../include/data/gameinfo.h"

GameInfo::ExeType GameInfo::get_exetype() {
    return target_exeType;
}

std::string GameInfo::get_label() {
    return target_label;
}

Workaround::Handler GameInfo::get_workaround() {
    return selected_workaround;
}
