//
// Created by cybik on 24-06-27.
//
#include <commands/HistoryCommand.h>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <iostream>

#include <QFile>
#include <QRegularExpression>

#include <termcolor/termcolor.hpp>

const QString HistoryCommand::CommandSpecifier = "history";

void HistoryCommand::process(int argc, char **argv) {
    QCoreApplication qwishes_history(argc, argv);
    QCoreApplication::setApplicationName(APPLICATION_NAME_GENERATOR(.history));
    QCoreApplication::setApplicationVersion(APP_VERSION);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument(
        "command",
        QCoreApplication::translate("main", "Command to run. MUST be History.")
    );

    std::shared_ptr<QCommandLineOption> game_path, reverse_order, open_url, max_return_num;
    parser.addOption(
        *(game_path = std::make_shared<QCommandLineOption>(
            QStringList() << "g" << "game_path",
            QCoreApplication::translate("history", "Path to the game installation."),
            "game_path",
            nullptr
        ))
    );
    parser.addOption(
        *(reverse_order = std::make_shared<QCommandLineOption>(
            QStringList() << "r" << "reverse_order",
            QCoreApplication::translate("history", "Return URLs in reversed order (from oldest to most recent).")
        ))
    );
    parser.addOption(
        *(open_url = std::make_shared<QCommandLineOption>(
            QStringList() << "o" << "open_url",
            QCoreApplication::translate("history", "Open URL in system browser.")
        ))
    );
    parser.addOption(
        *(max_return_num = std::make_shared<QCommandLineOption>(
            QStringList() << "m" << "max_return_num",
            QCoreApplication::translate("history", "Maximum number of URLs to return."),
            "max_return_num",
            "1"
        ))
    );

    parser.process(qwishes_history);

    if( parser.positionalArguments().empty() ||
        parser.positionalArguments()[0].compare(CommandSpecifier, Qt::CaseInsensitive) != 0
    ) {
        parser.showHelp(0);
    }

    if(!parser.isSet(*game_path)) {
        std::cout << "need game path oi" << std::endl;
        parser.showHelp(1);
    }

    this->command_game_path =       parser.value(*game_path);
    this->command_reverse_order =   parser.isSet(*reverse_order);   // if set, always true
    this->command_open_url =        parser.isSet(*open_url);        // if set, always true

    // Obsessed with oneliners, shut up.
    this->command_max_return_num = (parser.isSet(*max_return_num)?parser.value(*max_return_num).toInt():1);

    //std::cout << "ayyyyyyyyyyy :: " << this->command_game_path.toStdString() << std::endl;

    std::shared_ptr<std::list<std::shared_ptr<QFile>>> caches = this->getGameWishesCache();
    if(caches->empty()) {
        parser.showHelp(0);
    }
    for(const auto& qfile: *caches) {
        auto results = runFilterForLogs(runUrlCleanup(runUrlSearch(qfile)));
        // formatted, f*ck you
        std::cout
            << termcolor::bold << termcolor::cyan  << "[#] " << termcolor::reset
            << termcolor::bold << termcolor::green << "Data file" << termcolor::reset
            << ": "
            << termcolor::yellow << qfile->fileName().toStdString() << std::endl;
        if(results->empty()) {
            std::cout << termcolor::bold << termcolor::red << "No URLs read, detected or otherwise found." << std::endl;
            return;
        }
        if(this->command_max_return_num == 1 || results->size() == 1) {
            std::cout << (*results).front().to_stdstring() << std::endl;
            if(this->command_open_url) {
                // TODO: use xdg
            }
        } else {
            for(auto& staged: *results) {
                std::cout << "- " << staged.to_stdstring() << std::endl;
            }
        }
    }
}

std::shared_ptr<QStringList> HistoryCommand::runUrlSearch(const std::shared_ptr<QFile>& qfile) {
    if(!qfile->exists()) return nullptr;
    qfile->open(QFile::ReadOnly);
    QRegularExpression qreg("1/0/https(.*)\0\0\0\0\0\0\0\0");
    QRegularExpressionMatchIterator qrem = qreg.globalMatch(qfile->readAll());
    std::shared_ptr<QStringList> retList = std::make_shared<QStringList>();
    while(qrem.hasNext()) {
        retList->append(qrem.next().captured(0));
    }
    qfile->close();
    return retList;
}
std::shared_ptr<QStringList> HistoryCommand::runUrlCleanup(const std::shared_ptr<QStringList>& ptr) {
    std::shared_ptr<QStringList> retList = std::make_shared<QStringList>();
    for(const auto& single_string: (*ptr)) {
        for(const auto& split_string_1: single_string.split("1/0/")) { /** cut on 1/0/ **/
            if(split_string_1.startsWith("http")) {
                for(const auto& split_string_2: split_string_1.split('\0',Qt::SkipEmptyParts)) {
                    if(split_string_2.startsWith("http")) {
                        // always split after a cache entry, defined by nullchars
                        retList->append(split_string_2); // first of the second level
                    }
                }
            }
        }
    }
    ptr->clear(); /** cleanup **/
    return retList;
}

/**
 * https://gs.hoyoverse.com/nap/event/e20230424gacha/index.html?authkey_ver=1&sign_type=2&auth_appid=webview_gacha&win_mode=fullscreen&gacha_id=2c1f5692fdfbb733a08733f9eb69d32aed1d37&timestamp=1719887753&init_log_gacha_type=2001&init_log_gacha_base_type=2&ui_layout=&button_mode=default&plat_type=3&authkey=qEsfaOAGzSmE%2BfjT2l14NFp0K70%2Fd86qsRFNGhdkaGG6B5nAe00a%2FPZbTNgS0YvOYbdfUy9Ve%2BTxfGd0INMaTAE1%2Fwh3R9FcgpTAJqRypxokZ198SDQKDU3z%2B5JoZ%2FuT99LTTP1XeaG1wy3FT4XpDh9uCfqGYjecMejRCM7k2CdbdGAcRS2I5n5MGmBCjUET6m34uu4gTBgRZdlEkBNDfs%2BcFSixMydaFUnES7FwpdhkGXOm6QSq7JlvvZTnfKw0PYDlkJhVBlqgYvzdgqOA%2FDqYcPRgD%2F%2FektTZbj5Lc9y1%2FAuX9ax5TzBeszrdT%2BA%2BmdqGzhV6K9m%2B8C89oBRJOTTnervrpG8h2jmV%2BmUew86%2FqoCzaSF8kbBLWWvcotRLtVqhYYlK5H%2FWrMlntBsqKEZCS1pSjDMlDCiSHr5wUOljjsoOmnWhAYo5sjqTdG1KZ0Jj8qDmhYodd1eI0O8RNBk55auuKytopN2IgBwJ1O5scolIEjb3uUb6z%2BXOSSFvIqGY%2Fo8IqQWLnkdRoRhkrkTIql1%2BYNDpdOfghEOEW7V290T99PgNbCm%2BjPa1%2Bk5n6tG0quCltCVifpU1UCR2SFN0dUkIZVpmFzpQ4kNgcokv09XDcg0exOphilP5gG9ncMNXxZci%2FchwFqFywUsJY2wwIc6BjHccfigrYc6HFSM%3D&lang=en&region=prod_gf_us&game_biz=nap_global
 * */
std::shared_ptr<std::list<WishLog>> HistoryCommand::runFilterForLogs(const std::shared_ptr<QStringList>& ptr) {
    std::shared_ptr<std::list<WishLog>> retList = std::make_shared<std::list<WishLog>>();
    for(const auto& single_string: (*ptr)) {
        if(  single_string.contains("index.html") &&
            (single_string.contains("gacha-v")
                || (single_string.contains("game_biz=nap_global") && single_string.contains("gacha"))
            )
        ) {
            WishLog::WishLogGame wlg = WishLog::Genshin;
            if(single_string.contains("nap")) wlg = WishLog::ZZZ;
            retList->push_front(WishLog(single_string, WishLog::History, wlg));
        }
    }
    ptr->clear();
    return retList;
}