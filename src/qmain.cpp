#include <QCoreApplication>
#include <QCommandLineParser>

#include <algorithm>

//using namespace QWishes;

#include <commands/AbstractCommand.h>
#include <commands/HistoryCommand.h>
#include <commands/DataCommand.h>
#include <commands/LauncherCommand.h>
#include <commands/DaemonCommand.h>

#include <common.h>

#include <gachafs.h>
#include <gachasteam.h>

#define DEFINE_COMMAND(COMMNAME) \
    if(command.compare(COMMNAME::CommandSpecifier, Qt::CaseInsensitive) == 0) \
        return [](int argc, char** argv) { \
            return COMMNAME().cmd_main(argc, argv); \
        };

std::function<int(int, char**)> process_command_func(QString command) {
    DEFINE_COMMAND(HistoryCommand);
    DEFINE_COMMAND(LauncherCommand);
    DEFINE_COMMAND(DataCommand);
    DEFINE_COMMAND(DaemonCommand);
    return nullptr;
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

    int ret = -1;
    auto retf = process_command_func(parser.positionalArguments().at(0));
    if((ret = retf(argc, argv)) < 0) {
        parser.showHelp();
    }

    return ret;
}
