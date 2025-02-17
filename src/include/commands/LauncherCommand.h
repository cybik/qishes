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
#include <utility>

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
    std::shared_ptr<SARibbonCategory> getSocialsCat();

    std::shared_ptr<QMenu> tray_menu;
    std::shared_ptr<QAction> action_exit;
    std::shared_ptr<QAction> action_launch;
    std::shared_ptr<QSystemTrayIcon> tray;

    std::shared_ptr<QPixmap> icon;
    //std::shared_ptr<QAction> action_launcher;
    static std::shared_ptr<SettingsData> data;
    static std::unique_ptr<QAGL::Landing> landing;

    std::unique_ptr<SARibbonPannel> get_panel_run();
    std::unique_ptr<SARibbonPannel> get_panel_game();
    std::unique_ptr<SARibbonPannel> get_panel_wishes();
    std::unique_ptr<SARibbonPannel> get_panel_proton();
    std::unique_ptr<SARibbonPannel> get_panel_options();
    std::unique_ptr<SARibbonPannel> get_panel_socials();

    QAGL::QAGL_Game get_game();
    QAGL::QAGL_Game first_game_detected = QAGL::QAGL_Game::UNKNOWN;
    QAGL::QAGL_Game convert_exetype(ExeType target_type);
    void            create_fs_integration(const std::pair<ExeType, std::string>& inc, std::shared_ptr<QFile> file);

    // title shit
    std::shared_ptr<SARibbonMainWindow> given;

    std::shared_ptr<SARibbonCategory> given_cat;
    std::shared_ptr<SARibbonCategory> socials_cat;

    std::unique_ptr<SARibbonPannel> given_panel_options;
    std::unique_ptr<SARibbonCheckBox> given_option_obsvk;
    std::unique_ptr<SARibbonCheckBox> given_option_deckenv;
    std::unique_ptr<SARibbonCheckBox> given_option_cloudpc;
    std::unique_ptr<SARibbonCheckBox> given_option_mangohud;
    std::unique_ptr<SARibbonCheckBox> given_option_gamemode;
    std::unique_ptr<SARibbonCheckBox> given_option_auto_open_wishlog;

    std::unique_ptr<SARibbonPannel> given_panel_socials;
    std::unique_ptr<SARibbonCheckBox> given_option_discord;

    std::unique_ptr<SARibbonCheckBox> get_checkbox(QString title, QString objname, bool default_val = false);
    //std::unique_ptr<QAction> given_action_run;

    std::unique_ptr<SARibbonPannel> given_panel_run;
    std::unique_ptr<QAction> given_action_run;

    std::unique_ptr<SARibbonPannel> given_panel_game;
    std::unique_ptr<QAction> given_action_game;

    std::unique_ptr<SARibbonPannel> given_panel_wishes;
    std::unique_ptr<QAction> given_action_wishes;

    std::unique_ptr<SARibbonPannel> given_panel_proton;
    //std::unique_ptr<QAction> given_action_proton;
    std::unique_ptr<SARibbonComboBox> given_proton_combo;

    void remove_panel_and_action(   std::shared_ptr<SARibbonCategory>,
                                    std::unique_ptr<SARibbonPannel>,
                                    std::unique_ptr<QAction>
    );
    void show_wishes_getter();
    void checkDiscord();

    //
    QString target_exec;
    bool    exec_provided = false;

    std::map<int, std::string> target_execs_found;
    const std::map<std::string, std::pair<LauncherCommand::ExeType, std::string>> supported_games_impl = {
        {"launcher.exe", std::pair<ExeType, std::string>(ExeType::Launcher, "Launcher")},  // Rocket emoji
        {"GenshinImpact.exe", std::pair<ExeType, std::string>(ExeType::Genshin, "Genshin Impact")}, // Genshin Launcher Art
        {"StarRail.exe", std::pair<ExeType, std::string>(ExeType::HonkaiSR, "Honkai: Star Rail")},     // HSR Launcher Art
        {"ZenlessZoneZero.exe", std::pair<ExeType, std::string>(ExeType::Nap, "Zenless Zone Zero")},   // ZZZ Launcher Art
        {"BH3.exe", std::pair<ExeType, std::string>(ExeType::Honkai3rd, "Honkai: Impact 3rd")},         // BH3 Launcher Art
        // WuWa?
        // Nikki?
        // Some other anime boobfest?
        // Bueller?
    };
    std::list<std::shared_ptr<QAction>> actions_execs;
    std::shared_ptr<std::list<std::shared_ptr<QFile>>> filtered_files;

    void            enlist_launch_action(std::pair<LauncherCommand::ExeType, std::string> incoming, QString executable);

    bool command_offline;
    std::shared_ptr<QCommandLineOption> offline;

    bool command_steam;
    std::shared_ptr<QCommandLineOption> steam;

    void discord_report(QString message);

    std::shared_ptr<QFileSystemWatcher> qfsw = nullptr;
    std::unique_ptr<std::map<ExeType, QStringList>> base_lists = nullptr;

};


