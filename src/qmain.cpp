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
#include <steam_integration.h>

#include <cstdlib>

#include <unistd.h>

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

void enforce_qsg() {
    // Set QSG_RENDER_LOOP to basic only if it doesn't exist.
    setenv("QSG_RENDER_LOOP", "basic", 0);
    // Re-setup
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);
}

int detect_and_divert(int argc, char *argv[]) {
    if(steam_integration::get_steam_integration_instance()->running_under_steam()) {
        return 0;
    }
    return -1; // Base case: not being launched under Steam.
}

int main(int argc, char *argv[]) {
    if(int d_a_d = detect_and_divert(argc, argv); d_a_d >= 0) {
        Log::get_logger()->critical("running under steam, neato");
        setenv("QTWEBENGINE_DISABLE_SANDBOX", "1", 1);
    }

    // QT6 Linux workaround - somehow QSG Render Loop must be set for CLI apps that don't do windowing bollocks
    enforce_qsg();
    // for now, blast
    Log::get_logger()->log_level(Log::LogLevel::Debug);

    std::shared_ptr<QCoreApplication> qwishes = std::make_shared<QCoreApplication>(argc, argv);
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

    parser.process(*qwishes);

    if(parser.positionalArguments().empty()) {
        parser.showHelp(0);
    }
    qwishes->exit(0);
    qwishes.reset();

    int ret = -1;
    auto retf = process_command_func(parser.positionalArguments().at(0));
    if((ret = retf(argc, argv)) < 0) {
        parser.showHelp();
    }

    return ret;
}
