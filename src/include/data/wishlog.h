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
    WishLog(const QString &wishurl, WishLogType log_type);
    [[nodiscard]] std::string to_stdstring();
    [[nodiscard]] QString to_qstring();
    static bool is_accepted_url(const QString& url);
protected:
    WishLogGame guess_game(const QString& url);
private:
    QString log_url;
    int log_type;
    WishLogGame log_game;

    QString get_log_ext_url();
};