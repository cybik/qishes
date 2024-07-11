//
// Created by cybik on 24-07-03.
//

#include <data/wishlog.h>

WishLog::WishLog(const QString &wishurl, WishLogType log_type) {
    this->log_url = wishurl;
    this->log_type = log_type;
    this->log_game = guess_game(wishurl);
}

QString WishLog::get_log_ext_url() {
    if(log_type == WishLogType::History)
        if(log_game == WishLogGame::Genshin)
            return "#/log";
    return "";
}

std::string WishLog::to_stdstring() {
    return to_qstring().toStdString();
}

QString WishLog::to_qstring() {
    return (log_url + get_log_ext_url());
}

WishLog::WishLogGame WishLog::guess_game(const QString& url) {
    if(url.contains("nap")) return WishLogGame::ZZZ;
    if(url.contains("hkrpg")) return WishLogGame::HSR;
    // TODO: support WuWa?
    // TODO: support other games??
    return WishLog::Genshin;
}

bool WishLog::is_accepted_url(const QString& url) {
    // 0th Check: baselines event endpoint
    if(!url.contains("/event/")) return false;
    if(!url.contains("authkey=")) return false;

    // Game Check: HSR
    if(url.contains("hkrpg")) {
        if(url.contains("hkrpg_")) { // not _global, we want to care for our CN brethren
            return true;
        }
    }

    // Game Check: YuanShen
    if(url.contains("genshin")) {
        if(url.contains("hk4e_")) { // not _global, we want to care for our CN brethren
            return true;
        }
    }

    // Game Check: nap (ZZZ HA)
    if(url.contains("nap")) {
        if(url.contains("nap_")) { // not _global, we want to care for our CN brethren
            return true;
        }
    }

    return false;
}
