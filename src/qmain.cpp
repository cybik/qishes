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

static const QStringList list_commands = {{
    "history",
    "data",
    "daemon"
}};

std::unique_ptr<AbstractCommand> process_command(QString command) {
    if(command.compare(HistoryCommand::CommandSpecifier, Qt::CaseInsensitive) == 0) {
        return std::make_unique<HistoryCommand>();
    } else if(command.compare(LauncherCommand::CommandSpecifier, Qt::CaseInsensitive) == 0) {
        return std::make_unique<LauncherCommand>();
    } else if(command.compare(DataCommand::CommandSpecifier, Qt::CaseInsensitive) == 0) {
        return std::make_unique<DataCommand>();
    }
    return nullptr;
}

int main(int argc, char *argv[]) {
    QCoreApplication qwishes(argc, argv);
    QCoreApplication::setApplicationName(APPLICATION_NAME_GENERATOR());
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

    std::unique_ptr<AbstractCommand> command = process_command(
        parser.positionalArguments().at(0)
    );

    if(command == nullptr) {
        parser.showHelp();
    }

    command->cmd_main(argc, argv);
    //QCommandLineParser parser;
    //return QCoreApplication::exec();
    return 0;
}
