//
// Created by cybik on 24-06-27.
//
#include <commands/HistoryCommand.h>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <iostream>

#include <QFile>

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

    std::cout << "ayyyyyyyyyyy :: " << this->command_game_path.toStdString() << std::endl;

    this->getGameWishesCache();

    parser.showHelp(0);
}
