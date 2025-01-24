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

std::shared_ptr<Discord> Discord::clear() {
    Discord_ClearPresence();
    return _instance;
}

void Discord::dis_clear() {
    get_instance()->clear();
    Discord_Shutdown();
    _instance = nullptr;
}

std::shared_ptr<Discord> Discord::report_presence_message(QString msg) {
    DiscordRichPresence* drp = new DiscordRichPresence();
    drp->details = msg.toStdString().c_str();
    drp->startTimestamp = start.time_since_epoch().count();

    Discord_UpdatePresence(drp);
    return _instance;
}

void Discord::quit() {
    Discord_Shutdown();
}

void Discord::ready(const DiscordUser* r) { }

void Discord::disconnected(int errorCode, const char* message) { }

void Discord::errored(int errorCode, const char* message) { }

Discord::Discord() {
    this->deh = std::make_shared<DiscordEventHandlers>();
    this->deh->ready = &ready;
    this->deh->disconnected = &disconnected;
    this->deh->errored = &errored;
    Discord_Initialize(
        "478233407323897871",
        deh.get(),
        1,
        nullptr
    );
    start = std::chrono::system_clock::now();
}

