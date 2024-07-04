//
// Created by cybik on 24-07-03.
//

#include <data/wishlog.h>

WishLog::WishLog(const QString& wishurl, WishLogType log_type, WishLogGame log_game) {
    this->log_url = wishurl;
    this->log_type = log_type;
    this->log_game = log_game;
}

QString WishLog::get_log_ext_url() {
    if(log_type == WishLogType::History && log_game == WishLogGame::Genshin) return "#/log";
    return "";
}

std::string WishLog::to_stdstring() {
    return (log_url + get_log_ext_url()).toStdString();
}