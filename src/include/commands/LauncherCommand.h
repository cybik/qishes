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

#include "AbstractCommand.h"

#include <ui/Landing.h>
#include <util/Discord.h>

#include <core/qwkglobal.h>
#include <widgets/widgetwindowagent.h>

//#include <discord.h>
#include <SARibbon.h>

class LauncherCommand : public AbstractCommand {
public:
    static const QString CommandSpecifier;
    LauncherCommand() = default;

protected:
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
    void                     setupRibbonWindow();
    std::shared_ptr<QAction> get_action_launcher_test();
    std::shared_ptr<SARibbonCategory> getLauncherCat();

    std::shared_ptr<QMenu> tray_menu;
    std::shared_ptr<QAction> action_exit;
    std::shared_ptr<QAction> action_launch;
    std::shared_ptr<QSystemTrayIcon> tray;

    std::shared_ptr<Discord> dis;
    std::shared_ptr<QPixmap> icon;
    //std::shared_ptr<QAction> action_launcher;
    static std::shared_ptr<SettingsData> data;
    static std::unique_ptr<QAGL::Landing> landing;

    // title shit
    std::shared_ptr<SARibbonMainWindow> given;

    std::shared_ptr<SARibbonCategory> given_cat;

    std::unique_ptr<SARibbonPannel> given_panel_run;
    std::unique_ptr<QAction> given_action_run;

    std::unique_ptr<SARibbonPannel> given_panel_game;
    std::unique_ptr<QAction> given_action_game;

    std::unique_ptr<SARibbonPannel> given_panel_proton;
    //std::unique_ptr<QAction> given_action_proton;
    std::unique_ptr<SARibbonComboBox> given_proton_combo;

    void remove_panel_and_action(   std::shared_ptr<SARibbonCategory>,
                                    std::unique_ptr<SARibbonPannel>,
                                    std::unique_ptr<QAction>
    );

    //
    QString target_exec;
    bool    exec_provided_by_environment = false;
};


