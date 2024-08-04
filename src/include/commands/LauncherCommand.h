//
// Created by cybik on 24-07-10.
//

#pragma once

#include <QMenu>
#include "AbstractCommand.h"
#include <QSystemTrayIcon>
#include <QMainWindow>
#include <QDockWidget>

class LauncherCommand : public AbstractCommand {
public:
    static const QString CommandSpecifier;
    LauncherCommand() = default;
    int cmd_main(int, char **) override;

protected:
    void mbox();
private:
    std::shared_ptr<QApplication> this_app;
    std::shared_ptr<QMenu> generate_menu();
    std::shared_ptr<QSystemTrayIcon> generate_tray_icon();

    std::shared_ptr<QAction> get_action_exit();
    std::shared_ptr<QAction> get_action_rpc_ping();
    std::shared_ptr<QAction> get_action_dialog_test();
    std::shared_ptr<QAction> get_action_rpc_init();

    std::shared_ptr<QMenu> tray_menu;
    std::shared_ptr<QAction> action_exit;
    std::shared_ptr<QAction> action_init;
    std::shared_ptr<QAction> action_ping;
    std::shared_ptr<QAction> action_dial;
    std::shared_ptr<QSystemTrayIcon> tray;
    std::shared_ptr<discord::Activity> disco;
    std::shared_ptr<QMainWindow> qmw;
    std::shared_ptr<QDockWidget> qdw;

    //std::shared_ptr<QAction> action_launcher;
};


