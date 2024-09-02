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
        int current_page;
        int base_gacha_type; // Common to Genshin, HSR, Nap
        int real_gacha_type; // Nap-specific
        QString end_id;
    } WishLogPage; // Will likely get refactored elsewhere. For now, describes common shit to deduce pages from.
    typedef enum {
        History,
        Data
    } WishLogType;
    typedef enum {
        Genshin,
        HSR,
        ZZZ,

        Unsupported
    } WishLogGame;
    WishLog(const QString &wishurl, WishLogType log_type);
    [[nodiscard]] std::string to_stdstring();
    [[nodiscard]] QString to_qstring();

    WishLogGame game();

    QUrl getQuickInitUrl();
    QUrl regenerate_data_url(int target_gacha_type = -1, int target_gacha_page = 1, int target_end_id = -1);
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

    QString get_log_ext_url() const;
    void decode();
};