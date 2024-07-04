//
// Created by cybik on 24-07-03.
//

#pragma once

#include <QString>


class WishLog {
public:
    typedef enum {
        History,
        Data
    } WishLogType;
    typedef enum {
        Genshin,
        HSR,
        ZZZ
    } WishLogGame;
    WishLog(const QString& wishurl, WishLogType log_type, WishLogGame log_game);
    [[nodiscard]] std::string to_stdstring();
private:
    QString log_url;
    int log_type;
    WishLogGame log_game;

    QString get_log_ext_url();
};