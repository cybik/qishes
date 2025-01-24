/******************************************************************
 *
 * Discord.h
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-11-25, for qgachawishes.
 *
 ******************************************************************/

#pragma once

#include <discord_rpc.h>
#include <QString>

#include <memory>

class Discord {
public:
    static std::shared_ptr<Discord> get_instance();
    std::shared_ptr<Discord> report_presence_message(QString msg);
    static std::shared_ptr<Discord> clear();
    static void dis_clear();
    void quit();

    static void ready(const DiscordUser* r);
    static void disconnected(int errorCode, const char* message);
    static void errored(int errorCode, const char* message);

private:
    Discord();

    static std::shared_ptr<Discord> _instance;

    std::chrono::time_point<std::chrono::system_clock> start;

    std::shared_ptr<DiscordEventHandlers> deh;
};