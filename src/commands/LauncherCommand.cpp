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

#include <QWindow>

#include <QGuiApplication>
#include <QScreen>

#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QWebEnginePage>

#include <chrono>

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
std::shared_ptr<QAction> LauncherCommand::get_action_rpc_init() {
    action_init = std::make_shared<QAction>();

    action_init->setText("Test rpc init");
    QObject::connect(
        action_init.get(), &QAction::triggered, // Signal
        [&](bool) {
            std::shared_ptr<Discord> dis = Discord::get_instance();
        }
    );

    return action_init;
}

std::shared_ptr<QAction> LauncherCommand::get_action_rpc_ping() {
    action_ping = std::make_shared<QAction>();

    action_ping->setText("Test rpc ping");
    QObject::connect(
        action_ping.get(), &QAction::triggered, // Signal
        [&](bool) { }
    );

    return action_ping;
}

void LauncherCommand::mbox() {
    qmw = std::make_shared<QMainWindow>();
    qmw->setFixedSize(800, 600);
    qmw->setWindowTitle("Test Dialog");
    qmw->setWindowState(Qt::WindowActive);
    qmw->setMinimumSize(800, 600);
    qmw->setBaseSize(800, 600);
    qmw->move(
        QGuiApplication::primaryScreen()->geometry().center() - qmw->rect().center()
    );
    qmw->setVisible(true);
    qmw->show();
    qmw->raise();
    qmw->activateWindow();
}

void LauncherCommand::launcher() {
    if (!data) data = SettingsData::getSettingsData(); // todo: refresh
    if (!landing) {
        landing = std::make_unique<QAGL::Landing>(
            *this_app,
            std::move(data),
            QAGL::QAGL_App_Style::Normal
            //QAGL::QAGL_Game::h4ke, // Genshin
            //QAGL::QAGL_Region::global // Global
        );
    }
    QPixmap pix;
    pix.loadFromData(QByteArray::fromBase64(qiqi_smol.toLocal8Bit(), QByteArray::Base64Encoding));
    dis->report_presence_message("qishes on main");
    landing->show(*this_app);
}

void LauncherCommand::command_create_application(int& argc, char **argv) {
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    this_app = std::make_shared<QApplication>(argc, argv);
    QApplication::setApplicationName(APPNAME_GEN(.launcher));
    QApplication::setApplicationVersion(APP_VERSION);

    QApplication::connect(
        this_app.get(), &QApplication::aboutToQuit,
        [&]() {
            dis->quit();
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
    dis = Discord::get_instance();
    dis->report_presence_message("qishes loading");

    generate_tray_icon()->show();

    return this_app->exec();
}

std::shared_ptr<QAction> LauncherCommand::get_action_dialog_test() {
    action_dial = std::make_shared<QAction>();

    action_dial->setText("Test Dialog");
    QObject::connect(
        action_dial.get(), &QAction::triggered, // Signal
        [&](bool) {
            mbox();
        }
    );

    return action_dial;
}
std::shared_ptr<QAction> LauncherCommand::get_action_launcher_test() {
    action_launch = std::make_shared<QAction>();

    action_launch->setText("Test Launch");
    QObject::connect(
        action_launch.get(), &QAction::triggered, // Signal
        [&](bool) {
            launcher();
        }
    );

    return action_launch;
}

std::shared_ptr<QMenu> LauncherCommand::generate_menu() {
    tray_menu = std::make_shared<QMenu>();

    //tray_menu->addAction(get_action_rpc_init().get());
    //tray_menu->addAction(get_action_rpc_ping().get());
    //tray_menu->addAction(get_action_dialog_test().get());
    tray_menu->addAction(get_action_launcher_test().get());
    tray_menu->addAction(get_action_exit().get());
    //tray_menu->triggered(get_action_exit().get());

    return tray_menu;
}

std::shared_ptr<QSystemTrayIcon> LauncherCommand::generate_tray_icon() {
    tray = std::make_shared<QSystemTrayIcon>();
    std::cout << "Tray available? " << (tray->isSystemTrayAvailable()?"Yes":"No") << std::endl;
    tray->setContextMenu(generate_menu().get());
    QObject::connect(
            tray.get(), &QSystemTrayIcon::activated,
            [=](QSystemTrayIcon::ActivationReason trigger_event) {
                std::cout << "mikkikuu" << std::endl;
                tray->contextMenu()->show();
            }
    );

    return tray;
}
