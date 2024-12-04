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

std::shared_ptr<SARibbonCategory> LauncherCommand::getLauncherCat() {
    if (!given_panel_game) {
        given_action_game = std::make_shared<QAction>();
        given_action_game->setText("Select Launch Executable");
        given->connect(
            given_action_game.get(),
            &QAction::triggered,
            [&](bool) {
                target_exec = QFileDialog::getOpenFileName(
                    nullptr,
                    "Get me the genshin",
                    QString(std::getenv("STEAM_COMPAT_DATA_PATH")), "*.exe"
                );
            }
        );
        given_panel_game = std::make_shared<SARibbonPannel>();
        given_panel_game->addLargeAction(given_action_game.get());
    }
    if (!given_panel_proton) {
        given_action_proton = std::make_shared<QAction>();
        given_action_proton->setText("Select Proton");
        given->connect(
            given_action_proton.get(),
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
        given_panel_proton = std::make_shared<SARibbonPannel>();
        given_panel_proton->addLargeAction(given_action_proton.get());
    }
    if (!given_cat) {
        given_cat = std::make_shared<SARibbonCategory>();
        given_cat->setCategoryName("Game");
        given_cat->setObjectName("gamedata");

        given_cat->addPannel(given_panel_game.get());
        given_cat->addPannel(given_panel_proton.get());
    }
    return given_cat;
}

void LauncherCommand::setupRibbonWindow() {
    // Cool thing?
    given->setRibbonTheme(SARibbonTheme::RibbonThemeDark);
    given->ribbonBar()->setRibbonStyle(SARibbonBar::RibbonStyleCompactThreeRow);
    given->ribbonBar()->setMinimumMode(true);
    given->ribbonBar()->setTabOnTitle(true);
    given->ribbonBar()->setApplicationButton(nullptr);
    given->ribbonBar()->addCategoryPage(getLauncherCat().get());

    given->windowButtonBar()->setupMaximizeButton(false);
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
    QApplication::setApplicationName(APPNAME_GEN(.launcher));
    QApplication::setApplicationVersion(APP_VERSION);

    (icon = std::make_shared<QPixmap>())
        ->loadFromData(QByteArray::fromBase64(qiqi_smol.toLocal8Bit(), QByteArray::Base64Encoding));

    QApplication::connect(
        this_app.get(), &QApplication::aboutToQuit,
        [&]() {
            // Discord yeet
            dis.reset();

            // Panel yeet
            given_cat->removePannel(given_panel_proton.get());
            given_panel_proton->removeAction(given_action_proton.get());
            given_panel_proton.reset();

            // Panel yeet
            given_cat->removePannel(given_panel_game.get());
            given_panel_game->removeAction(given_action_game.get());
            given_panel_game.reset();

            // Ribbon reset
            given_cat.reset();

            // Landing window yeet
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
