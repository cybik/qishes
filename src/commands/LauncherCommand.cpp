//
// Created by cybik on 24-07-10.
//

#include <commands/LauncherCommand.h>

#include <QApplication>


#include <QDialog>
#include <QObject>

#include <iostream>

const QString LauncherCommand::CommandSpecifier = "launcher";

std::shared_ptr<QAction> LauncherCommand::get_action_exit() {
    action_exit = std::make_shared<QAction>();

    action_exit->setText("Test");
    QObject::connect(
            action_exit.get(), &QAction::triggered, // Signal
            [=](bool) {
                std::cout << "mikkiku" << std::endl;
                abort();
            }
    );

    return action_exit;
}

std::shared_ptr<QMenu> LauncherCommand::generate_menu() {
    tray_menu = std::make_shared<QMenu>();

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

void LauncherCommand::cmd_main(int argc, char **argv) {
    this_app = std::make_shared<QApplication>(argc, argv);
    QApplication::setApplicationName(APPLICATION_NAME_GENERATOR(.launcher));
    QApplication::setApplicationVersion(APP_VERSION);

    generate_tray_icon()->show();

    this_app->exec();

    abort();
}
