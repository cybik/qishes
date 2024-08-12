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
                if(item.first == "init_type")      base_gacha_type              = item.second;
            }
            break;
        }
        case(WishLog::HSR): {
            break;
        }
        case(WishLog::ZZZ): {
            for(const auto& item: que.queryItems()) {
                if(item.first == "init_log_gacha_type")      real_gacha_type    = item.second;
                if(item.first == "init_log_gacha_base_type") base_gacha_type    = item.second;
            }
        }
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

QUrl WishLog::regenerate_data_url(int target_gacha_type) {
    // TODO: make this more nimble to be able to hit one method call with page, end_id
    // TODO: huge chain calls for init_type:*
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
            log_data_page = 1;
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

            if(missing_page)  reprocess_query.addQueryItem("page", QString::number(log_data_page));
            if(missing_size)  reprocess_query.addQueryItem("size", QString::number(log_data_page_size));
            if(missing_end_id) reprocess_query.addQueryItem("end_id", end_id);

            // Regenerating a "fresh" url
            if(reset_init_type) {
                reprocess_query.removeQueryItem("init_type");
                reprocess_query.addQueryItem("init_type", QString::number(target_gacha_type));
            }
            reprocess_query.addQueryItem("gacha_type", base_gacha_type);
            break;
        }
        case(WishLog::HSR): {
            data_url.setHost("api-os-takumi.mihoyo.com");
            data_url.setPath("/common/gacha_record/api/getGachaLog");
            /* TODO: fill in missing query bits */
            break;
        }
        case(WishLog::ZZZ): {
            data_url.setHost("public-operation-nap-sg.hoyoverse.com");
            data_url.setPath("/common/gacha_record/api/getGachaLog");
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

/**
 * https://gs.hoyoverse.com/genshin/event/e20190909gacha-v3/index.html?
 *      win_mode=fullscreen&authkey_ver=1&sign_type=2&auth_appid=webview_gacha&init_type=301&gacha_id=3da267d03aea30f566b528fe6f52c8d23050d6f3&timestamp=1717544785&lang=en&device_type=pc&game_version=OSRELWin4.7.0_R24292157_S24332145_D24803766&region=os_usa&authkey=GW2JPu5AetePHbO5YYC%2f89zxvXuF6USUDFkKS6gxSqzPRXnykyseD5BU1UecIMDFcERGwVvMMPwjT%2bfK9RKveCEKCYpKv%2bW1mthEwmDb0%2bG0UeTpncZ8jndo870r0pOoosuQ%2f3afHDvtvs3tEiGFNaelbg9%2b6Suhxt%2fEpJ%2fnVRxZr0p%2bp%2fJqLSlmtkKsty8Vc8c16Y1GDWX89bUaJMNKq1XLTSybnO5WiFiZZyVSTd%2f2Q1sM%2fpJtpy%2btGRWrZU9zUj8EFyOVj5A6tluyGYyVBNw06JqYvcA%2bYhoRtt6XHY82IqXowRfDzEhvDdDWGxBoB2x39O1QOX20OLfk3lXAU4ytf5WssJxP9Js8%2fva4asIXm3027f7gyP3G6ymO2TGOlZMSxj%2fpEn0aFVx7rqh9LVax27QKQFLYwD%2fAgIbhmCtKAiGps8MQ31eTCeVHALKcddG2hcguDWRATlG18f0ur%2fDBFlV3emWX%2frgKiuFzRzrTNwJ6B1mj%2fq5XVdRWN1ZQ1x3pYL74qNxEDt7lMzkNx9AcaioCg4RN83zfCGm5wKIpVlwqeq7gnLFUf7rag82wvij1Tw9XHr%2bNH4V0L8RV%2bGzUTbJdk2eqZniME5LCAXwYyDG9FQNG5XZ3s2EDGN6Dff4SRBVxtFcVeAXPVbweg1Q560jQDdf1jwypqc7O5SqEMK8DB5g6GnulLREjcOL9chZI4uhBO4ahlI2BcSDvMex%2b%2fvgoia7wcuTze5Jg9BOz170UVjeKmqlRsXhHzuchvqHAgeLKvWWIZM6voN9%2flSqyHkaX%2f6EjoDtC7zH1YBJVB93xXzQbxnFm1AzZXVALAgnbOqeOX5z9Dpx%2fhdX3hGNVWpbbMu%2f3A2NdykfyaS4Y4YAfoexxnKuu5zraYpiHzoXO3JJybHKRnpwHySxvc5mPZ7GNOwpQ8OmiPKyF%2fAZUAixnGyYkRlMw7J2t7Wn3b5UF07ds4Khzoewdw%2fYs0XPF587nXJAjBO1hq7h9pkDONggKF5PS3RKJBz15EBuQp%2bQPon68bVHH357fJ6Fcwb2KEhOiQgX9UFu%2bXgXgU7FVht3eG7EyTrZ5uivvFcEVWT8Mwqt3rn%2fgCt5QQsZ%2fwqfWH3sh4hiruf5BTtk50%2bzmcWc%2bqZh0eJV6KLj5FAEOyXT6HIieuoYbsTqup7syqmqvTWc9ySVK%2f8ilMmemqU3z43%2b9AWOhmQo%2fiRGw9Iab6mBX%2f5xY3t8CNfQHHcuwuKUrSif%2fr3RU5No2nv%2bX6HQbQRAk8B1eFdUFyj2fSEMw8r5sOHdUBSdoQin9d2sO207kZ8Nd8BkMrZJMQcrHcs%2bpgHlO6CIvDIFHV0kqrsjdJjSY%2fWz0C6zGkOamvqU1Ow%3d%3d&game_biz=hk4e_global
 *      win_mode=fullscreen&authkey_ver=1&sign_type=2&auth_appid=webview_gacha&init_type=301&gacha_id=3da267d03aea30f566b528fe6f52c8d23050d6f3&timestamp=1717544785&lang=en&device_type=pc&game_version=OSRELWin4.7.0_R24292157_S24332145_D24803766&region=os_usa&authkey=GW2JPu5AetePHbO5YYC%2f89zxvXuF6USUDFkKS6gxSqzPRXnykyseD5BU1UecIMDFcERGwVvMMPwjT%2bfK9RKveCEKCYpKv%2bW1mthEwmDb0%2bG0UeTpncZ8jndo870r0pOoosuQ%2f3afHDvtvs3tEiGFNaelbg9%2b6Suhxt%2fEpJ%2fnVRxZr0p%2bp%2fJqLSlmtkKsty8Vc8c16Y1GDWX89bUaJMNKq1XLTSybnO5WiFiZZyVSTd%2f2Q1sM%2fpJtpy%2btGRWrZU9zUj8EFyOVj5A6tluyGYyVBNw06JqYvcA%2bYhoRtt6XHY82IqXowRfDzEhvDdDWGxBoB2x39O1QOX20OLfk3lXAU4ytf5WssJxP9Js8%2fva4asIXm3027f7gyP3G6ymO2TGOlZMSxj%2fpEn0aFVx7rqh9LVax27QKQFLYwD%2fAgIbhmCtKAiGps8MQ31eTCeVHALKcddG2hcguDWRATlG18f0ur%2fDBFlV3emWX%2frgKiuFzRzrTNwJ6B1mj%2fq5XVdRWN1ZQ1x3pYL74qNxEDt7lMzkNx9AcaioCg4RN83zfCGm5wKIpVlwqeq7gnLFUf7rag82wvij1Tw9XHr%2bNH4V0L8RV%2bGzUTbJdk2eqZniME5LCAXwYyDG9FQNG5XZ3s2EDGN6Dff4SRBVxtFcVeAXPVbweg1Q560jQDdf1jwypqc7O5SqEMK8DB5g6GnulLREjcOL9chZI4uhBO4ahlI2BcSDvMex%2b%2fvgoia7wcuTze5Jg9BOz170UVjeKmqlRsXhHzuchvqHAgeLKvWWIZM6voN9%2flSqyHkaX%2f6EjoDtC7zH1YBJVB93xXzQbxnFm1AzZXVALAgnbOqeOX5z9Dpx%2fhdX3hGNVWpbbMu%2f3A2NdykfyaS4Y4YAfoexxnKuu5zraYpiHzoXO3JJybHKRnpwHySxvc5mPZ7GNOwpQ8OmiPKyF%2fAZUAixnGyYkRlMw7J2t7Wn3b5UF07ds4Khzoewdw%2fYs0XPF587nXJAjBO1hq7h9pkDONggKF5PS3RKJBz15EBuQp%2bQPon68bVHH357fJ6Fcwb2KEhOiQgX9UFu%2bXgXgU7FVht3eG7EyTrZ5uivvFcEVWT8Mwqt3rn%2fgCt5QQsZ%2fwqfWH3sh4hiruf5BTtk50%2bzmcWc%2bqZh0eJV6KLj5FAEOyXT6HIieuoYbsTqup7syqmqvTWc9ySVK%2f8ilMmemqU3z43%2b9AWOhmQo%2fiRGw9Iab6mBX%2f5xY3t8CNfQHHcuwuKUrSif%2fr3RU5No2nv%2bX6HQbQRAk8B1eFdUFyj2fSEMw8r5sOHdUBSdoQin9d2sO207kZ8Nd8BkMrZJMQcrHcs%2bpgHlO6CIvDIFHV0kqrsjdJjSY%2fWz0C6zGkOamvqU1Ow%3d%3d&game_biz=hk4e_global&gacha_type=301&page=2&size=5&end_id=1719356760001218273
 * https://hk4e-api-os.hoyoverse.com/gacha_info/api/getGachaLog?
 **/