/******************************************************************
 *
 * Discord.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-11-25, for qgachawishes.
 *
 ******************************************************************/

#include <chrono>
#include <util/Discord.h>

#include "discord_rpc.h"

std::shared_ptr<Discord> Discord::_instance = nullptr;

std::shared_ptr<Discord> Discord::get_instance() {
    if ( !_instance ) {
        _instance = std::shared_ptr<Discord>(new Discord());
    }

    return _instance;
}

void Discord::report_presence_message(QString msg) {
    DiscordRichPresence* drp = new DiscordRichPresence();
    drp->details = msg.toStdString().c_str();
    drp->startTimestamp = start.time_since_epoch().count();

    Discord_UpdatePresence(drp);
}

void Discord::quit() {
    Discord_Shutdown();
}


Discord::Discord() {
    Discord_Initialize(
        "478233407323897871",
        nullptr,
        1,
        nullptr
    );
    start = std::chrono::system_clock::now();
}

