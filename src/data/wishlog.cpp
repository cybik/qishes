//
// Created by cybik on 24-07-03.
//

#include <data/wishlog.h>

#include <util/log.h>

WishLog::WishLog(const QString &wishurl, WishLogType log_type) {
    this->log_url_str = wishurl;
    this->log_type = log_type;
    this->log_game = guess_game(wishurl);

    this->log_url = QUrl(log_url_str);
}

QString WishLog::get_log_ext_url() {
    if(log_type == WishLogType::History) {
        if (log_game == WishLogGame::Genshin) { return "#/log"; }
        else if (log_game == WishLogGame::ZZZ) { return "#/info"; }
    }
    return "";
}

std::string WishLog::to_stdstring() {
    return to_qstring().toStdString();
}

QString WishLog::to_qstring() {
    return (log_url_str + get_log_ext_url());
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
    if(url.contains("hkrpg"))
        if(url.contains("hkrpg_"))  // not _global, we want to care for our CN brethren
            return true;

    // Game Check: YuanShen
    if(url.contains("genshin"))
        if(url.contains("hk4e_"))   // not _global, we want to care for our CN brethren
            return true;

    // Game Check: nap (ZZZ HA)
    if(url.contains("nap"))
        if(url.contains("nap_"))    // not _global, we want to care for our CN brethren
            return true;

    return false;
}

QUrl WishLog::regenerate_data_url() {
    QUrl data_url = QUrl(log_url);
    QUrlQuery reprocess_query = QUrlQuery(log_url.query());
    //Log::get_logger()->critical(reprocess_query.toString());
    switch(log_game) {
        case(WishLog::Genshin): {
            data_url.setHost("hk4e-api-os.hoyoverse.com");
            data_url.setPath("/gacha_info/api/getGachaLog");
            /* TODO: fill in missing query bits */
            break;
        }
        case(WishLog::HSR):{
            data_url.setHost("api-os-takumi.mihoyo.com");
            data_url.setPath("/common/gacha_record/api/getGachaLog");
            /* TODO: fill in missing query bits */
            break;
        }
        case(WishLog::ZZZ): {
            data_url.setHost("public-operation-nap-sg.hoyoverse.com");
            data_url.setPath("/common/gacha_record/api/getGachaLog");
            /* TODO: fill in missing query bits */
            /**
             *          &page=1
             *          &size=5
             *          &gacha_type=2001
             *          &real_gacha_type=2
             *          &end_id=
             **/
            log_data_page = 1;
            // identify missing;
            bool    missing_page            = true,
                    missing_size            = true,
                    missing_gacha_type      = true,
                    missing_real_gacha_type = true,
                    missing_end_id          = true;
            for(const auto& item: reprocess_query.queryItems()) {
                if(item.first == "init_log_gacha_type")      real_gacha_type    = item.second;
                if(item.first == "init_log_gacha_base_type") base_gacha_type    = item.second;

                if(item.first == "page")            missing_page                = false;
                if(item.first == "size")            missing_size                = false;
                if(item.first == "gacha_type")      missing_gacha_type          = false;
                if(item.first == "real_gacha_type") missing_real_gacha_type     = false;
                if(item.first == "end_id")          missing_end_id              = false;
            }
            if(missing_page) reprocess_query.addQueryItem("page", QString::number(log_data_page));
            if(missing_size)  reprocess_query.addQueryItem("size", QString::number(log_data_page_size));
            if(missing_gacha_type) reprocess_query.addQueryItem("gacha_type", real_gacha_type);
            if(missing_real_gacha_type) reprocess_query.addQueryItem("real_gacha_type", base_gacha_type);
            if(missing_end_id) reprocess_query.addQueryItem("end_id", end_id);

            break;
        }
    }
    data_url.setQuery(reprocess_query);
    //Log::get_logger()->critical(data_url.query());
    return data_url;
}
