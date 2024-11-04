/******************************************************************
 *
 * wishlog.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-07-03 for qgachawishes.
 *
 ******************************************************************/

#include <data/wishlog.h>

#include <util/log.h>

WishLog::WishLog(const QString &wishurl, WishLogType log_type) {
    this->log_url_str = wishurl;
    this->log_type = log_type;
    this->log_game = guess_game(wishurl);

    this->log_url = QUrl(log_url_str);

    this->decode();
}

QString WishLog::get_log_ext_url() const {
    if(log_type == WishLogType::History)
        if (log_game == WishLogGame::Genshin)
            return "#/log";

    return "";
}

void WishLog::decode() {
    QUrlQuery que = QUrlQuery(log_url.query());

    switch(log_game) {
        case(WishLog::Genshin): {
            for(const auto& item: que.queryItems()) {
                if(item.first == "init_type")               base_gacha_type              = item.second;
            }
            break;
        }
    case(WishLog::HSR): {
            for(const auto& item: que.queryItems()) {
                if(item.first == "default_gacha_type")      base_gacha_type              = item.second;
            }
            break;
        }
        case(WishLog::ZZZ): {
            for(const auto& item: que.queryItems()) {
                if(item.first == "init_log_gacha_type")      real_gacha_type    = item.second;
                if(item.first == "init_log_gacha_base_type") base_gacha_type    = item.second;
            }
        }
        case(WishLog::Unsupported): abort();
    }
}


std::string WishLog::to_stdstring() {
    return to_qstring().toStdString();
}

QString WishLog::to_qstring() {
    return (log_url_str + get_log_ext_url());
}

WishLog::WishLogGame WishLog::game() {
    return this->log_game;
}

WishLog::WishLogGame WishLog::guess_game(const QString& url) {
    if(url.contains("nap")) return WishLogGame::ZZZ;
    if(url.contains("hkrpg")) return WishLogGame::HSR;
    if(url.contains("hk4e_")) return WishLogGame::Genshin;
    // TODO: support WuWa?
    // TODO: support other games??
    return WishLog::Unsupported;
}

QString WishLog::get_identified_game_name() {
    switch(this->game()) {
    case WishLog::Genshin: return "Genshin Impact";
    case WishLog::HSR:     return "Honkai: Star Rail";
    case WishLog::ZZZ:     return "Zenless Zone Zero";
    default: abort();
    }
}

bool WishLog::is_accepted_url(const QString& url) {
    // 0th Check: baselines event endpoint check
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

QUrl WishLog::getQuickInitUrl() {
    QUrl interim = regenerate_data_url();
    QUrlQuery interim_query = QUrlQuery(interim.query());
    for(auto item: QList<QPair<QString,QString>>({{"page","1"}, {"size","1"}, {"end_id",""}})) {
        interim_query.removeQueryItem(item.first);
        interim_query.addQueryItem(item.first, item.second);
    }
    interim.setQuery(interim_query);
    return interim;
}

QUrl WishLog::regenerate_data_url(
    int target_gacha_type,
    int target_gacha_page,
    const QString& target_end_id
) {
    // TODO: make this more nimble to be able to hit one method call with page, end_id
    // TODO: huge chain calls for init_type:*

    // probably the lifecycle of a referenced variable in an odd setup getting nuked.
    // maybe start throwing around shared_ptrs or even unique_ptrs in this context.
    QUrl data_url = QUrl(log_url);
    QUrlQuery reprocess_query = QUrlQuery(log_url.query());
    //Log::get_logger()->critical(reprocess_query.toString());
    switch(log_game) {
        case(WishLog::Genshin): {
            //data_url.setHost("hk4e-api-os.hoyoverse.com");
            data_url.setHost("public-operation-hk4e-sg.hoyoverse.com");
            data_url.setPath("/gacha_info/api/getGachaLog");
            bool    missing_page            = true,
                    missing_size            = true,
                    missing_end_id          = true,
                    reset_init_type         = false
            ;
            ld_page = target_gacha_page;
            for(const auto& item: reprocess_query.queryItems()) {
                if(item.first == "page")        missing_page    = false;
                if(item.first == "size")        missing_size    = false;
                if(item.first == "end_id")      missing_end_id  = false;

                if(item.first == "init_type") {
                    base_gacha_type = (target_gacha_type!=-1?QString::number(target_gacha_type):item.second);
                    if(target_gacha_type != -1 && item.second.toInt() != target_gacha_type) {
                        reset_init_type = true;
                    }
                }
            }

            if(missing_page)  reprocess_query.addQueryItem("page", QString::number(ld_page));
            if(missing_size)  reprocess_query.addQueryItem("size", QString::number(ld_page_size));

            // Regenerating a "fresh" url
            if(reset_init_type) {
                reprocess_query.removeQueryItem("init_type");
                reprocess_query.addQueryItem("init_type", QString::number(target_gacha_type));
            }

            // specify end_id override. or reset
            if(target_end_id != end_id) {
                if(!missing_end_id) reprocess_query.removeQueryItem("end_id");
                end_id = target_end_id;
                missing_end_id = true;
            }
            if(missing_end_id) reprocess_query.addQueryItem("end_id", end_id);

            reprocess_query.addQueryItem("gacha_type", base_gacha_type);
            break;
        }
        case(WishLog::HSR): {
            //data_url.setHost("api-os-takumi.mihoyo.com");
            data_url.setHost("public-operation-hkrpg-sg.hoyoverse.com");
            data_url.setPath("/common/gacha_record/api/getGachaLog");
            bool    missing_page            = true,
                    missing_size            = true,
                    missing_end_id          = true,
                    reset_init_type         = false
            ;
            /* TODO: fill in missing query bits */
            // TODO: missing gacha_type matching upstream default_gacha_type
            ld_page = target_gacha_page;
            // identify missing;
            bool    missing_gacha_type      = true;
            for(const auto& item: reprocess_query.queryItems()) {
                if(item.first == "default_gacha_type")      default_gacha_type      = item.second;
                if(item.first == "gacha_type")              missing_gacha_type      = false;
                if(item.first == "page")                    missing_page            = false;
                if(item.first == "size")                    missing_size            = false;
                if(item.first == "end_id")                  missing_end_id          = false;

                if(item.first == "default_gacha_type") {
                    base_gacha_type = (target_gacha_type!=-1?QString::number(target_gacha_type):item.second);
                    if(target_gacha_type != -1 && item.second.toInt() != target_gacha_type) {
                        reset_init_type = true;
                    }
                }
            }

            if(missing_gacha_type) reprocess_query.addQueryItem("gacha_type", default_gacha_type);
            if(missing_page)  reprocess_query.addQueryItem("page", QString::number(ld_page));
            if(missing_size)  reprocess_query.addQueryItem("size", QString::number(ld_page_size));

            // Regenerating a "fresh" url
            if(reset_init_type) {
                reprocess_query.removeQueryItem("gacha_type");
                reprocess_query.addQueryItem("gacha_type", QString::number(target_gacha_type));
                reprocess_query.removeQueryItem("default_gacha_type");
                reprocess_query.addQueryItem("default_gacha_type", QString::number(target_gacha_type));
            }

            // specify end_id override. or reset
            if(target_end_id != end_id) {
                if(!missing_end_id) reprocess_query.removeQueryItem("end_id");
                end_id = target_end_id;
                missing_end_id = true;
            }
            if(missing_end_id) reprocess_query.addQueryItem("end_id", end_id);

            break;
        }
        case(WishLog::ZZZ): {
            data_url.setHost("public-operation-nap-sg.hoyoverse.com");
            data_url.setPath("/common/gacha_record/api/getGachaLog");
            ld_page = 1;
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
            if(missing_page)            reprocess_query.addQueryItem("page", QString::number(ld_page));
            if(missing_size)            reprocess_query.addQueryItem("size", QString::number(ld_page_size));
            if(missing_gacha_type)      reprocess_query.addQueryItem("gacha_type", real_gacha_type);
            if(missing_real_gacha_type) reprocess_query.addQueryItem("real_gacha_type", base_gacha_type);
            if(missing_end_id)          reprocess_query.addQueryItem("end_id", end_id);

            break;
        }
        case(WishLog::Unsupported): abort();
    }
    data_url.setQuery(reprocess_query);
    Log::get_logger()->debug(data_url.query());
    return data_url;
}
