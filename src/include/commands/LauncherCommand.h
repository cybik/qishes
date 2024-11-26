/******************************************************************
 *
 * LauncherCommand.h
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-07-10 for qgachawishes.
 *
 ******************************************************************/

#pragma once

#include <QMenu>
#include "AbstractCommand.h"
#include <QSystemTrayIcon>
#include <QMainWindow>
#include <QDockWidget>

#include <ui/Landing.h>
#include <util/Discord.h>

//#include <discord.h>

class LauncherCommand : public AbstractCommand {
public:
    static const QString CommandSpecifier;
    LauncherCommand() = default;

protected:
    void mbox();
    void launcher();
    void command_create_application(int& argc, char **argv) override;
    void command_setup_parser() override;
    void command_process_parser() override;
    int  command_run() override;
private:
    std::shared_ptr<QApplication> this_app = nullptr;
    std::shared_ptr<QMenu> generate_menu();
    std::shared_ptr<QSystemTrayIcon> generate_tray_icon();

    std::shared_ptr<QAction> get_action_exit();
    std::shared_ptr<QAction> get_action_rpc_ping();
    std::shared_ptr<QAction> get_action_dialog_test();
    std::shared_ptr<QAction> get_action_launcher_test();
    std::shared_ptr<QAction> get_action_rpc_init();

    std::shared_ptr<QMenu> tray_menu;
    std::shared_ptr<QAction> action_exit;
    std::shared_ptr<QAction> action_init;
    std::shared_ptr<QAction> action_ping;
    std::shared_ptr<QAction> action_dial;
    std::shared_ptr<QAction> action_launch;
    std::shared_ptr<QSystemTrayIcon> tray;
    std::shared_ptr<QMainWindow> qmw;
    std::shared_ptr<QDockWidget> qdw;

    std::shared_ptr<Discord> dis;
    //std::shared_ptr<QAction> action_launcher;
    static std::shared_ptr<SettingsData> data;
    static std::unique_ptr<QAGL::Landing> landing;
};


