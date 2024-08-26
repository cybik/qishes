#include <QCoreApplication>
#include <QCommandLineParser>

#include <algorithm>

//using namespace QWishes;

#include <commands/AbstractCommand.h>
#include <commands/HistoryCommand.h>
#include <commands/DataCommand.h>
#include <commands/LauncherCommand.h>

#include <common.h>

#include <gachafs.h>
#include <gachasteam.h>

#include "commands/DaemonCommand.h"

static const QStringList list_commands = {{
    "history",
    "data",
    "daemon"
}};

int process_command(QString command, int argc, char** argv) {
    if(command.compare(HistoryCommand::CommandSpecifier, Qt::CaseInsensitive) == 0)
        return HistoryCommand().cmd_main(argc, argv);
    if(command.compare(LauncherCommand::CommandSpecifier, Qt::CaseInsensitive) == 0)
        return LauncherCommand().cmd_main(argc, argv);
    if(command.compare(DataCommand::CommandSpecifier, Qt::CaseInsensitive) == 0)
        return DataCommand().cmd_main(argc, argv);
    if(command.compare(DaemonCommand::CommandSpecifier, Qt::CaseInsensitive) == 0)
        return DaemonCommand().cmd_main(argc, argv);
    return -1;
}

int main(int argc, char *argv[]) {
    // for now, blast
    Log::get_logger()->log_level(Log::LogLevel::Debug);

    QCoreApplication qwishes(argc, argv);
    QCoreApplication::setApplicationName(APPNAME_GEN());
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QCoreApplication::setOrganizationDomain("cybik.moe");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument(
        "command",
        QCoreApplication::translate("main", "Command to run.")
    );

    parser.setOptionsAfterPositionalArgumentsMode(QCommandLineParser::ParseAsPositionalArguments);

    parser.process(qwishes);

    if(parser.positionalArguments().empty()) {
        parser.showHelp(0);
    }

    const int ret = process_command(parser.positionalArguments().at(0), argc, argv);
    if(ret < 0) {
        parser.showHelp();
    }

    return ret;
}
