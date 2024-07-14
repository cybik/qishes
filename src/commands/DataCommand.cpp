//
// Created by cybik on 24-07-10.
//

#include <commands/DataCommand.h>
#include <QCoreApplication>
#include <QCommandLineParser>

#include <common.h>

const QString DataCommand::CommandSpecifier = "data";

void DataCommand::cmd_main(int argc, char **argv) {
    QCoreApplication qwishes_data(argc, argv);
    QCoreApplication::setApplicationName(APPLICATION_NAME_GENERATOR(.data));
    QCoreApplication::setApplicationVersion(APP_VERSION);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument(
        "command",
        QCoreApplication::translate("main", "Command to run. MUST be data.")
    );

    std::shared_ptr<QCommandLineOption> game_path, known_url;
    parser.addOption(
        *(game_path = std::make_shared<QCommandLineOption>(
            QStringList() << "g" << "game_path",
            QCoreApplication::translate(
                CommandSpecifier.toStdString().c_str(), "Path to the game installation. May or may not be provided."
            ),
            "game_path",
            nullptr
        ))
    );
    parser.addOption(
        *(known_url = std::make_shared<QCommandLineOption>(
            QStringList() << "u" << "known_url",
            QCoreApplication::translate(
                CommandSpecifier.toStdString().c_str(), "Known URL. May or may not be provided."
            ),
            "known_url_path",
            nullptr
        ))
    );
    parser.process(qwishes_data);

    this->command_game_path =   parser.value(*game_path);
    this->command_known_url =   parser.value(*known_url);   // if set, always true

    if(command_known_url.isEmpty() && command_game_path.isEmpty()) {
        Log::get_logger()->warning("No game path nor known URL was provided.");
        Log::get_logger()->warning("One of the two must be provided.");
        parser.showHelp(2);
    }

    abort();
}