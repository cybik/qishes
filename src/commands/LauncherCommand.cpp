//
// Created by cybik on 24-07-10.
//

#include <commands/LauncherCommand.h>

#include <QApplication>


#include <QObject>

#include <iostream>

#include <QWindow>


const QString LauncherCommand::CommandSpecifier = "launcher";

std::shared_ptr<QAction> LauncherCommand::get_action_exit() {
    action_exit = std::make_shared<QAction>();

    action_exit->setText("Test yeet");
    QObject::connect(
            action_exit.get(), &QAction::triggered, // Signal
            [&](bool) {
                std::cout << "mikkiku" << std::endl;
                abort();
            }
    );

    return action_exit;
}
std::shared_ptr<QAction> LauncherCommand::get_action_rpc_init() {
    action_init = std::make_shared<QAction>();

    action_init->setText("Test init");
    QObject::connect(
        action_init.get(), &QAction::triggered, // Signal
        [&](bool) {
            std::cout << "mikkiku" << std::endl;

            disco = std::make_shared<discord::Activity>();
            disco->SetApplicationId(478233407323897871);
        }
    );

    return action_init;
}

std::shared_ptr<QAction> LauncherCommand::get_action_rpc_ping() {
    action_ping = std::make_shared<QAction>();

    action_ping->setText("Test rpc");
    QObject::connect(
        action_ping.get(), &QAction::triggered, // Signal
        [&](bool) {
            disco->SetState("Bumming Around");
            disco->SetType(discord::ActivityType::Playing);
            disco->SetName("Derping Around");
        }
    );

    return action_ping;
}

void LauncherCommand::mbox() {

    qmw = std::make_shared<QMainWindow>(nullptr, Qt::Window);
    qmw->setFixedSize(800, 600);
    qmw->setWindowTitle("Test Dialog");
    qmw->setWindowState(Qt::WindowActive);
    qmw->setBaseSize(800, 600);
    qmw->show();
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

std::shared_ptr<QMenu> LauncherCommand::generate_menu() {
    tray_menu = std::make_shared<QMenu>();

    tray_menu->addAction(get_action_rpc_init().get());
    tray_menu->addAction(get_action_rpc_ping().get());
    tray_menu->addAction(get_action_dialog_test().get());
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
                abort();
            }
    );

    return tray;
}

int LauncherCommand::cmd_main(int argc, char **argv) {
    this_app = std::make_shared<QApplication>(argc, argv);
    QApplication::setApplicationName(APPNAME_GEN(.launcher));
    QApplication::setApplicationVersion(APP_VERSION);

    generate_tray_icon()->show();

    this_app->exec();

    abort();
    return 0;
}
