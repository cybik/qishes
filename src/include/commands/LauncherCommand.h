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
    ~LauncherCommand() = default;

protected:
    void launcher();
    void command_create_application(int& argc, char **argv) override;
    void command_setup_parser() override;
    void command_process_parser() override;
    int  command_run() override;
private:
    typedef enum {
        Genshin,
        HonkaiSR,
        Honkai3rd,
        Nap,
        Launcher,

        WutheringWaves, /** Unsupported for now */
        InfinityNikki,  /** Unsupported for now */
        Strinova,       /** Unsupported for now */

        Unknown         /** NYANEEEEEEEEEEEEEEH */
    } ExeType;
    void run_the_magic(const QString& target_exec);

    std::shared_ptr<QApplication> qishes_launcher = nullptr;
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

    std::unique_ptr<SARibbonPannel> get_panel_run();
    std::unique_ptr<SARibbonPannel> get_panel_game();
    std::unique_ptr<SARibbonPannel> get_panel_options();
    std::unique_ptr<SARibbonPannel> get_panel_proton();

    // title shit
    std::shared_ptr<SARibbonMainWindow> given;

    std::shared_ptr<SARibbonCategory> given_cat;

    std::unique_ptr<SARibbonPannel> given_panel_options;
    std::unique_ptr<SARibbonCheckBox> given_option_mangohud;
    std::unique_ptr<SARibbonCheckBox> given_option_deckenv;
    std::unique_ptr<SARibbonCheckBox> given_option_obsvk;
    std::unique_ptr<SARibbonCheckBox> given_option_cloudpc;

    std::unique_ptr<SARibbonCheckBox> get_checkbox(QString title, QString objname, bool default_val = false);
    //std::unique_ptr<QAction> given_action_run;

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
    bool    exec_provided = false;

    std::map<int, std::string> target_execs_found;
    const std::map<std::string, LauncherCommand::ExeType> supported_games_impl = {
        {"launcher.exe", ExeType::Launcher},
        {"GenshinImpact.exe", ExeType::Genshin},
        {"StarRail.exe", ExeType::HonkaiSR},
        {"ZenlessZoneZero.exe", ExeType::Nap},
        {"BH3.exe", ExeType::Honkai3rd},
        // WuWa?
        // Nikki?
        // Some other anime boobfest?
        // Bueller?
    };
    std::list<std::shared_ptr<QAction>> actions_execs;
    std::shared_ptr<std::list<std::shared_ptr<QFile>>> filtered_files;

    void enlist_launch_action(QString message, QString executable);

    bool command_offline;
    std::shared_ptr<QCommandLineOption> offline;

};


