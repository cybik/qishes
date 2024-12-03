/******************************************************************
 *
 * LauncherCommand.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-07-10 for qgachawishes.
 *
 ******************************************************************/

#include <commands/LauncherCommand.h>

#include <QApplication>

#include <QObject>

#include <iostream>

#include <QWebEngineView>

#include <chrono>
#include "steam_integration.h"

const QString LauncherCommand::CommandSpecifier = "launcher";

std::shared_ptr<SettingsData> LauncherCommand::data = nullptr;
std::unique_ptr<QAGL::Landing> LauncherCommand::landing = nullptr;

std::shared_ptr<QAction> LauncherCommand::get_action_exit() {
    action_exit = std::make_shared<QAction>();

    action_exit->setText("Test yeet");
    QObject::connect(
            action_exit.get(), &QAction::triggered, // Signal
            [&](bool) {
                std::cout << "mikkikue" << std::endl;
                QApplication::quit();
            }
    );

    return action_exit;
}

void LauncherCommand::setupRibbonWindow() {
    // Cool thing?
    given->setRibbonTheme(SARibbonTheme::RibbonThemeDark);
    given->ribbonBar()->setRibbonStyle(SARibbonBar::RibbonStyleCompactTwoRow);
    given->ribbonBar()->setMinimumMode(true);
    given->ribbonBar()->setTabOnTitle(true);
    given->ribbonBar()->applicationButton()->setText("moo");
    given->ribbonBar()->applicationButton()->setToolTip("moo");

    given->windowButtonBar()->setupMaximizeButton(false);
    given->windowButtonBar()->connect(
    given->windowButtonBar()
            ->addAction("Proton", QIcon(), Qt::ToolButtonTextOnly),
        &QAction::triggered,
        [&](bool) {
            QStringList list;
            for (auto str: vlvproton::getInstance()->get_available_protons()) list << str.c_str();
            auto proton = QInputDialog::getItem(
                nullptr,
                "Choose wisely",
                "Proton",
                list
            );
            steam_integration::get_steam_integration_instance()->proton()->select(proton.toStdString());
        }
        );
    given->windowButtonBar()->connect(
    given->windowButtonBar()
            ->addAction("Executable", QIcon(), Qt::ToolButtonTextOnly),
        &QAction::triggered,
        [&](bool) {
            target_exec = QFileDialog::getOpenFileName(
                nullptr,
                "Get me the genshin",
                QString(std::getenv("STEAM_COMPAT_DATA_PATH")), "*.exe"
            );
        }
    );
    //given->windowButtonBar()->setWindowTitle("test");
}

void LauncherCommand::launcher() {
    if (!data) data = SettingsData::getSettingsData(); // todo: refresh
    if (!landing) {
        given = std::make_shared<SARibbonMainWindow>();

        landing = std::make_unique<QAGL::Landing>(
            *this_app,
            std::move(data),
            QAGL::QAGL_App_Style::Normal,
            QAGL::QAGL_Game::h4ke, // Genshin
            QAGL::QAGL_Region::global, // Global
            given
        );

        setupRibbonWindow();
        landing->hint_titlebar_height(given->ribbonBar()->titleBarHeight());

    }
    dis->report_presence_message("qishes on main");

    landing->show(*this_app);
}

void LauncherCommand::command_create_application(int& argc, char **argv) {
    // Quirk: Early detection of Bootstrap steam environment
    if (auto clientlaunch = std::getenv("SteamClientLaunch") ;
        std::getenv("SteamUser") &&
            (! clientlaunch || strcmp(clientlaunch, "1") == 0)
    ) {
        exit(0);
    }

    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings); // QWindowKit
    this_app = std::make_shared<QApplication>(argc, argv);
    _Argc = argc;
    for (int i = 0; i < argc; i++) {
        _Argv.append(argv[i]);
    }
    QApplication::setApplicationName(APPNAME_GEN(.launcher));
    QApplication::setApplicationVersion(APP_VERSION);

    (icon = std::make_shared<QPixmap>())
        ->loadFromData(QByteArray::fromBase64(qiqi_smol.toLocal8Bit(), QByteArray::Base64Encoding));

    QApplication::connect(
        this_app.get(), &QApplication::aboutToQuit,
        [&]() {
            dis.reset();
            landing.reset();
        }
    );
}

void LauncherCommand::command_setup_parser() {

}

void LauncherCommand::command_process_parser() {

}

int LauncherCommand::command_run() {
    dis = Discord::get_instance()->report_presence_message("qishes loading");

    generate_tray_icon()->show();

    return this_app->exec();
}

std::shared_ptr<QAction> LauncherCommand::get_action_launcher_test() {
    action_launch = std::make_shared<QAction>();

    action_launch->setText("Test Launch");
    QObject::connect(
        action_launch.get(), &QAction::triggered, // Signal
        [&](bool) { launcher(); }
    );

    return action_launch;
}

std::shared_ptr<QMenu> LauncherCommand::generate_menu() {
    tray_menu = std::make_shared<QMenu>();
    tray_menu->addActions( { get_action_launcher_test().get(), get_action_exit().get() } );
    return tray_menu;
}

std::shared_ptr<QSystemTrayIcon> LauncherCommand::generate_tray_icon() {
    tray = std::make_shared<QSystemTrayIcon>();
    tray->setContextMenu(generate_menu().get());
    tray->setIcon(*icon);
    QObject::connect(
        tray.get(), &QSystemTrayIcon::activated,
        [&](QSystemTrayIcon::ActivationReason) { tray->contextMenu()->show(); }
    );

    return tray;
}
