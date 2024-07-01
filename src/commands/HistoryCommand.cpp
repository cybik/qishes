//
// Created by cybik on 24-06-27.
//
#include <commands/HistoryCommand.h>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <iostream>

#include <QFile>
#include <QRegularExpression>

const QString HistoryCommand::CommandSpecifier = "history";

HistoryCommand::HistoryCommand() {
    // noop for now
}

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
    if(caches->size() <= 0) {
        parser.showHelp(0);
    }
    foreach(std::shared_ptr<QFile> qfile, *caches) {
        std::shared_ptr<QStringList> results = runFilterForLogs(runUrlCleanup(runUrlSearch(qfile)));
        std::cout << "[#] Data file: " << qfile->fileName().toStdString() << std::endl;
        if(this->command_max_return_num == 1 || results->size() == 1) {
            std::cout << (*results)[0].toStdString() << std::endl;
        } else {
            foreach(auto staged, *results) {
                std::cout<<"- " << staged.toStdString() << std::endl;
            }
        }
    }
}

std::shared_ptr<QStringList> HistoryCommand::runUrlSearch(std::shared_ptr<QFile> qfile) {
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
std::shared_ptr<QStringList> HistoryCommand::runUrlCleanup(std::shared_ptr<QStringList> ptr) {
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
std::shared_ptr<QStringList> HistoryCommand::runFilterForLogs(std::shared_ptr<QStringList> ptr) {
    std::shared_ptr<QStringList> retList = std::make_shared<QStringList>();
    for(const auto& single_string: (*ptr)) {
        if(single_string.contains("gacha-v") && single_string.contains("index.html")) {
            retList->push_front(single_string);
        }
    }
    ptr->clear();
    return retList;
}