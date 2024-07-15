//
// Created by cybik on 24-07-03.
//

#pragma once

#include <QString>

#include <QUrl>
#include <QUrlQuery>

class WishLog {
public:
    typedef struct {

    } WishLogPage;
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
    QUrl regenerate_data_url();
    QUrl regenerate_preview_url();
    static bool is_accepted_url(const QString& url);
protected:
    WishLogGame guess_game(const QString& url);
private:
    QString log_url_str;
    int log_type;
    WishLogGame log_game;

    QUrl log_url;
    QUrl log_data_url;
    int  log_data_page          = -1;
    int  log_data_page_size     = 10;
    QString  real_gacha_type    = ""; // TODO: This is unique to ZZZ
    QString  base_gacha_type    = ""; // TODO: This is unique to ZZZ
    QString  end_id             = ""; // will be used

    QString get_log_ext_url();
};