//
// Created by cybik on 24-07-10.
//

#pragma once

#include <QMenu>
#include "AbstractCommand.h"
#include <QSystemTrayIcon>

class LauncherCommand : public AbstractCommand {
public:
    static const QString CommandSpecifier;
    LauncherCommand() = default;
    void cmd_main(int, char **) override;

protected:
private:
    std::shared_ptr<QApplication> this_app;
    std::shared_ptr<QMenu> generate_menu();
    std::shared_ptr<QSystemTrayIcon> generate_tray_icon();
    std::shared_ptr<QAction> get_action_exit();

    std::shared_ptr<QMenu> tray_menu;
    std::shared_ptr<QAction> action_exit;
    std::shared_ptr<QSystemTrayIcon> tray;
    //std::shared_ptr<QAction> action_launcher;
};


