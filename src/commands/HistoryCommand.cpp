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

    std::shared_ptr<QCommandLineOption> game_path, file_path, reverse_order, open_url, max_return_num;
    parser.addOption(
        *(game_path = std::make_shared<QCommandLineOption>(
            QStringList() << "g" << "game_path",
            SPEC_TRANSLATE("Path to the game installation."),
            "game_path",
            nullptr
        ))
    );
    parser.addOption(
        *(file_path = std::make_shared<QCommandLineOption>(
            QStringList() << "f" << "file_path",
            SPEC_TRANSLATE("Path to the cache file directly."),
            "file_path",
            nullptr
        ))
    );
    parser.addOption(
        *(reverse_order = std::make_shared<QCommandLineOption>(
            QStringList() << "r" << "reverse_order",
            SPEC_TRANSLATE("Return URLs in reversed order (from oldest to most recent).")
        ))
    );
    parser.addOption(
        *(open_url = std::make_shared<QCommandLineOption>(
            QStringList() << "o" << "open_url",
            SPEC_TRANSLATE("Open URL in system browser.")
        ))
    );
    parser.addOption(
        *(max_return_num = std::make_shared<QCommandLineOption>(
            QStringList() << "m" << "max_return_num",
            SPEC_TRANSLATE("Maximum number of URLs to return."),
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

    this->command_game_path =       parser.value(*game_path);
    this->command_file_path =       parser.value(*file_path);
    this->command_reverse_order =   parser.isSet(*reverse_order);   // if set, always true
    this->command_open_url =        parser.isSet(*open_url);        // if set, always true

    // Obsessed with oneliners, shut up.
    this->command_max_return_num = (parser.isSet(*max_return_num)?parser.value(*max_return_num).toInt():1);

    //std::cout << "ayyyyyyyyyyy :: " << this->command_game_path.toStdString() << std::endl;
    std::shared_ptr<std::list<std::shared_ptr<QFile>>> caches;
    if(!this->command_file_path.isEmpty() && QFileInfo::exists(command_file_path)) {
        caches = std::make_shared<std::list<std::shared_ptr<QFile>>>();
        caches->emplace_front(std::make_shared<QFile>(QFileInfo(command_file_path).absoluteFilePath()));
    } else if(!this->command_game_path.isEmpty()) {
        caches = this->getGameWishesCache(&parser);
    } else {
        Log::get_logger()->critical("No good source of information was provided to extract a history URL from.");
        parser.showHelp(5);
    }

    for(const auto& qfile: *caches) {
        // Header. C++ yet formatted, f*ck you.
        printSingleFilePath(qfile->fileName());

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
            if(command_reverse_order) results->reverse();
            for(auto& staged: *results) {
                std::cout << "- " << staged.to_stdstring() << std::endl;
            }
        }
    }
}

