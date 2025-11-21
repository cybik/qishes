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

#include <AGame.h>
#include <GameInfo.h>

#include <Launcher.h>

#include <httpclient/httpclient.hpp>

#include <commands/controls/LauncherControlCb.h>

class LauncherCommand : public AbstractCommand {
public:
    static const QString CommandSpecifier;
    LauncherCommand();// = default;
    ~LauncherCommand();

protected:
    void launcher();
    void command_create_application(int& argc, char **argv) override;
    void command_setup_parser() override;
    void command_process_parser() override;
    int  command_run() override;
private:
    void quit();
    void run_the_magic(std::shared_ptr<AGame>);

    std::shared_ptr<QApplication>    qishes_launcher = nullptr;
    std::shared_ptr<HttpClient>                              mHttpClient;
    std::shared_ptr<QMenu>           generate_menu();
    std::shared_ptr<QSystemTrayIcon> generate_tray_icon();

    std::shared_ptr<QAction> get_action_exit();
    void                     setupRibbonWindow(std::shared_ptr<SARibbonMainWindow> target);
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
    std::unique_ptr<SARibbonPannel> get_panel_wine();
    std::unique_ptr<SARibbonPannel> get_panel_game();
    std::unique_ptr<SARibbonPannel> get_panel_wishes();
    std::unique_ptr<SARibbonPannel> get_panel_proton();
    std::unique_ptr<SARibbonPannel> get_panel_options();
    std::unique_ptr<SARibbonPannel> get_panel_socials();

    void updateConfig();

    QAGL::QAGL_Game get_game();
    QAGL::QAGL_Game first_game_detected = QAGL::QAGL_Game::GAME_UNKNOWN;
    QAGL::QAGL_Game convert_exetype(GameInfo::ExeType target_type);
    void            create_fs_integration(GameInfo::ExeType, std::filesystem::path filepath);

    // title shit
    std::shared_ptr<SARibbonMainWindow> given;
    std::shared_ptr<SARibbonMainWindow> given2;
    void setupAuxiliary();

    std::shared_ptr<QPixmap> loadBackdrop();

    std::shared_ptr<SARibbonCategory> given_cat;
    std::shared_ptr<SARibbonCategory> socials_cat;

    std::unique_ptr<SARibbonPannel> given_panel_options;

    std::unique_ptr<LauncherControlCb> given_option_mangohud;
    std::unique_ptr<LauncherControlCb> given_option_obsvk;
    std::unique_ptr<LauncherControlCb> given_option_deckenv;
    std::unique_ptr<LauncherControlCb> given_option_steamosenv;
    std::unique_ptr<LauncherControlCb> given_option_wayland;
    std::unique_ptr<LauncherControlCb> given_option_no_deco;
    std::unique_ptr<LauncherControlCb> given_option_xalia;

    std::unique_ptr<SARibbonCheckBox> given_option_cloudpc;
    std::unique_ptr<SARibbonCheckBox> given_option_gamemode;
    std::unique_ptr<SARibbonCheckBox> given_option_auto_open_wishlog;
    std::unique_ptr<SARibbonCheckBox> given_option_steamwrap;

    std::unique_ptr<SARibbonPannel> given_panel_socials;
    std::unique_ptr<SARibbonCheckBox> given_option_discord;

    std::unique_ptr<SARibbonCheckBox> get_checkbox(QString title, QString objname, bool default_val = false);
    //std::unique_ptr<QAction> given_action_run;

    std::unique_ptr<SARibbonPannel> given_panel_run;
    std::unique_ptr<QAction> given_action_run;

    std::unique_ptr<SARibbonPannel> given_panel_wine;

    //std::unique_ptr<SARibbonPannel> given_panel_game;
    //std::unique_ptr<QAction> given_action_game;

    /*
    std::unique_ptr<SARibbonPannel> given_panel_wishes;
    std::unique_ptr<QAction> given_action_wishes;
    */

    std::unique_ptr<SARibbonPannel> given_panel_proton;
    //std::unique_ptr<QAction> given_action_proton;
    std::unique_ptr<SARibbonComboBox> given_proton_combo;

    void remove_panel_and_action(   std::shared_ptr<SARibbonCategory>,
                                    std::unique_ptr<SARibbonPannel>,
                                    std::unique_ptr<QAction>
    );
    void show_wishes_getter();
    void checkDiscord();

    void process_env_cb(std::map<std::string, std::string>&, std::unique_ptr<LauncherControlCb>& cb);
    void process_steam_launchwrap(std::unique_ptr<SARibbonCheckBox>& cb);

    //
    //QString target_exec;
    std::shared_ptr<AGame> main_exec;
    std::shared_ptr<AGame> first_game;
    bool    exec_provided = false;

    std::map<int, std::string> target_execs_found;
    std::map<std::string, std::shared_ptr<AGame>> supported_games;
    std::list<std::shared_ptr<QAction>> actions_execs; // ?? custom and game?
    std::list<std::shared_ptr<QAction>> wine_execs; // ?? custom and game?

    // todo: clean up filtered files, this is unsightly as HELL.
    std::shared_ptr<std::list<std::pair<std::shared_ptr<AGame>, std::shared_ptr<QFile>>>> filtered_files;
    std::shared_ptr<std::list<std::pair<std::shared_ptr<AGame>, std::filesystem::path>>> filtered_files_;

    void            enlist_launch_action(std::shared_ptr<AGame> aGame);
    void            enlist_custom_action(QString, QString, std::list<std::shared_ptr<QAction>>*);

    bool command_offline;
    std::shared_ptr<QCommandLineOption> offline;

    bool command_steam;
    std::shared_ptr<QCommandLineOption> steam;

    void discord_report(QString message);

    std::shared_ptr<QFileSystemWatcher> qfsw = nullptr;
    std::unique_ptr<std::map<GameInfo::ExeType, QStringList>> base_lists = nullptr;

};


