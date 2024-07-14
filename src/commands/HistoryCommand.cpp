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

#include <QDesktopServices>
#include <QUrl>

const QString HistoryCommand::CommandSpecifier = "history";

void HistoryCommand::cmd_main(int argc, char **argv) {
    QCoreApplication qwishes_history(argc, argv);
    QCoreApplication::setApplicationName(APPLICATION_NAME_GENERATOR(.history));
    QCoreApplication::setApplicationVersion(APP_VERSION);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument(
        "command",
        QCoreApplication::translate("main", "Command to run. MUST be history.")
    );

    std::shared_ptr<QCommandLineOption> game_path, reverse_order, open_url, max_return_num;
    parser.addOption(
        *(game_path = std::make_shared<QCommandLineOption>(
            QStringList() << "g" << "game_path",
            QCoreApplication::translate(
                CommandSpecifier.toStdString().c_str(), "Path to the game installation."
            ),
            "game_path",
            nullptr
        ))
    );
    parser.addOption(
        *(reverse_order = std::make_shared<QCommandLineOption>(
            QStringList() << "r" << "reverse_order",
            QCoreApplication::translate(
                CommandSpecifier.toStdString().c_str(), "Return URLs in reversed order (from oldest to most recent)."
            )
        ))
    );
    parser.addOption(
        *(open_url = std::make_shared<QCommandLineOption>(
            QStringList() << "o" << "open_url",
            QCoreApplication::translate(
                CommandSpecifier.toStdString().c_str(), "Open URL in system browser."
            )
        ))
    );
    parser.addOption(
        *(max_return_num = std::make_shared<QCommandLineOption>(
            QStringList() << "m" << "max_return_num",
            QCoreApplication::translate(
                CommandSpecifier.toStdString().c_str(), "Maximum number of URLs to return."
            ),
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
        Log::get_logger()->warning("No game path was provided. Cannot deduce the path.");
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
        // Header. C++ yet formatted, f*ck you.
        std::cout
            << termcolor::bold << termcolor::cyan  << "[#] "        << termcolor::reset
            << termcolor::bold << termcolor::green << "Data file"   << termcolor::reset
            << ": "
            << termcolor::yellow << qfile->fileName().toStdString() << termcolor::reset
            << std::endl;

        auto results = runFilterForLogs(runUrlCleanup(runUrlSearch(qfile)));
        if(!results || results->empty()) {
            Log::get_logger()->critical("No URLs read, detected or otherwise found.");
            return;
        }
        if(this->command_max_return_num == 1 || results->size() == 1) {
            std::cout << (*results).front().to_stdstring() << std::endl;
            if(this->command_open_url) {
                // seems qt doesn't want to allow use desktop services in cli
                //QDesktopServices::openUrl(QUrl((*results).front().to_qstring()));
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
    std::shared_ptr<QStringList> retList; // don't initialize unless necessary
    for(const auto& single_string: (*ptr)) {
        for(const auto& split_string_1: single_string.split("1/0/")) { /** cut on 1/0/ **/
            for(const auto& split_string_2: split_string_1.split('\0', Qt::SkipEmptyParts)) {
                if(split_string_2.startsWith("http")) {
                    if(!retList) retList = std::make_shared<QStringList>();
                    // always split after a cache entry, defined by nullchars
                    retList->append(split_string_2); // first of the second level
                }
            }
        }
    }
    ptr->clear(); /** cleanup **/
    return retList;
}

std::shared_ptr<std::list<WishLog>> HistoryCommand::runFilterForLogs(const std::shared_ptr<QStringList>& ptr) {
    if(!ptr) return nullptr;
    std::shared_ptr<std::list<WishLog>> retList = std::make_shared<std::list<WishLog>>();
    for(const auto& single_string: (*ptr)) {
        if(WishLog::is_accepted_url(single_string)) {
            retList->emplace_front(single_string, WishLog::History);
        }
    }
    ptr->clear();
    return retList;
}