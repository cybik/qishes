/******************************************************************
 *
 * LauncherCommand.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-07-10 for qgachawishes.
 *
 * TODOs
 *  * Add "Open drive_c in xdg explorer" button
 *  * Refactor fswatcher to (re?)run on game launch
 *    * fswatcher cleanup?
 *  * Default game setter if more than one game is detected
 *
 ******************************************************************/

#include <commands/LauncherCommand.h>

#include <QApplication>

#include <QObject>

#include <iostream>

#include <chrono>
#include <steam_integration.h>

#include <gachafs.h>
#include <wine.h>
#include <sys/socket.h>

#include <ui/dwishes.h>

#include <data/wishlog.h>

const QString LauncherCommand::CommandSpecifier = "launcher";

std::shared_ptr<SettingsData> LauncherCommand::data = nullptr;
std::unique_ptr<QAGL::Landing> LauncherCommand::landing = nullptr;

std::shared_ptr<QAction> LauncherCommand::get_action_exit() {
    action_exit = std::make_shared<QAction>();

    action_exit->setText("Quit");
    QObject::connect(
        action_exit.get(), &QAction::triggered, // Signal
        [&](bool) {
            QApplication::quit();
        }
    );

    return action_exit;
}

std::unique_ptr<SARibbonCheckBox> LauncherCommand::get_checkbox(QString title, QString objname, bool default_val) {
    std::unique_ptr<SARibbonCheckBox> cb = std::make_unique<SARibbonCheckBox>();
    cb->setText(title);
    cb->setObjectName(objname);
    cb->setChecked(default_val);
    return std::move(cb);
}

std::unique_ptr<SARibbonPannel> LauncherCommand::get_panel_options() {
    given_option_mangohud = std::move(get_checkbox(
        "MangoHUD", "cbMango", true)
    );
    given_option_deckenv = std::move(get_checkbox(
        "Fakeout Deck", "cbDeckMode", true)
    );
    given_option_obsvk = std::move(get_checkbox(
        "OBS VkCapture Mode", "cbVkCap", true)
    );
    given_option_cloudpc = std::move(get_checkbox(
        "Cloud Masquerade", "cbImpersonateCloud", true)
    );
    given_option_gamemode = std::move(get_checkbox(
        "GameModeRun", "cbGameMode", true)
        );
    given_option_auto_open_wishlog = std::move(get_checkbox(
        "Auto-Open new Wish Log Entries", "cbWishlog", true)
    );

    std::unique_ptr<SARibbonPannel> panel_opt = std::make_unique<SARibbonPannel>();
    panel_opt->addSmallWidget(given_option_mangohud.get());
    panel_opt->addSmallWidget(given_option_deckenv.get());
    panel_opt->addSmallWidget(given_option_obsvk.get());
    panel_opt->addSmallWidget(given_option_cloudpc.get());
    panel_opt->addSmallWidget(given_option_gamemode.get());
    panel_opt->addSmallWidget(given_option_auto_open_wishlog.get());
    panel_opt->setPannelName("Options");
    return std::move(panel_opt);
}

void LauncherCommand::checkDiscord() {
    if (!given_option_discord->isChecked()) {
        Discord::dis_clear();
    } else {
        this->discord_report("qishes on main");
    }
}

std::unique_ptr<SARibbonPannel> LauncherCommand::get_panel_socials() {
    given_option_discord = std::move(get_checkbox("Discord", "cbDiscord", false));

    QObject::connect(
        given_option_discord.get(), &QCheckBox::toggled,
        [&](bool) {
            this->checkDiscord();
        }
    );

    std::unique_ptr<SARibbonPannel> panel_socials = std::make_unique<SARibbonPannel>();
    panel_socials->addSmallWidget(given_option_discord.get());
    panel_socials->setPannelName("Social Integrations");
    return std::move(panel_socials);
}

std::unique_ptr<SARibbonPannel> LauncherCommand::get_panel_proton() {
    given_proton_combo = std::make_unique<SARibbonComboBox>();
    given_proton_combo->setWindowTitle("ProtonSelect");
    given_proton_combo->setObjectName("ProtonSelect");
    // TODO: set current selected to match config that's not implemented yet
    std::unique_ptr<SARibbonPannel> panel_proton = std::make_unique<SARibbonPannel>();
    panel_proton->addSmallWidget(given_proton_combo.get());
    panel_proton->setPannelName("Proton Runtime");
    for (auto str: vlvproton::getInstance()->get_available_protons()) {
        given_proton_combo->addItem(QString(str.c_str()));
    }
    return std::move(panel_proton);
}

void LauncherCommand::run_the_magic(const QString& target_exe) {
    std::map<std::string, std::string> envs = {};
    std::list<std::string> arguments = {};
    std::string true_command = target_exe.toStdString();

    if (given_option_mangohud->isChecked()) envs["MANGOHUD"] = "1";
    if (given_option_deckenv->isChecked())  envs["SteamDeck"] = "1";
    if (given_option_obsvk->isChecked())    envs["OBS_VKCAPTURE"] = "1";
    if (given_option_cloudpc->isChecked()) {
        arguments.emplace_back("-platform_type");
        arguments.emplace_back("CLOUD_THIRD_PARTY_PC");
    }
    if (given_option_gamemode->isChecked()) {
        arguments.emplace_front(true_command);
    }
    steam_integration::get_steam_integration_instance()->proton()->try_run(
        target_exe.toStdString(), arguments, envs,
        (given_option_gamemode->isChecked()?"gamemoderun":"")
    );
}

void LauncherCommand::enlist_launch_action(
    std::string incoming, QString executable
) {
    std::shared_ptr<QAction> action_run = std::make_unique<QAction>(incoming.c_str());
    given->connect(
        action_run.get(),
        &QAction::triggered,
        [&, executable](bool) {
            steam_integration::get_steam_integration_instance()->proton()->select(
                given_proton_combo->currentText().toStdString()
            );
            if (!executable.isEmpty()) {
                run_the_magic(executable);
            }
        }
    );
    actions_execs.emplace_back(action_run);
}

QAGL::QAGL_Game LauncherCommand::convert_exetype(GameInfo::ExeType target_type) {
    switch (target_type) {
        case GameInfo::ExeType::Genshin: return QAGL::QAGL_Game::h4ke;
        case GameInfo::ExeType::HonkaiSR: return QAGL::QAGL_Game::hkrpg;
        case GameInfo::ExeType::WutheringWaves: return QAGL::QAGL_Game::wuwa;
        case GameInfo::ExeType::Honkai3rd: return QAGL::QAGL_Game::bh3 ;
        case GameInfo::ExeType::Nap: return QAGL::QAGL_Game::nap;
        default: return QAGL::QAGL_Game::UNKNOWN;
    }
}

void LauncherCommand::create_fs_integration(GameInfo::ExeType inc, std::shared_ptr<QFile> file) {
    switch(inc) {
        case(GameInfo::ExeType::WutheringWaves):
            abort(); // not supported yet but coming.
        case(GameInfo::ExeType::Genshin):
        case(GameInfo::ExeType::HonkaiSR):
        case(GameInfo::ExeType::Nap): {
            // Initial FS watcher. DO NOT RE-CREATE.
            if (!qfsw) {
                // QFileSystemWatcher on all data_2 present.
                // TODO: make it so each game itself, gets its watcher.
                auto caches = getGameWishesCache(
                    QString(file->filesystemFileName().parent_path().c_str())
                );
                if (caches->empty()) {
                    return; // Don't watch since there's nothing.
                }
                // var: detected urls
                if (!detected_urls) detected_urls = std::make_shared<QStringList>();
                for (auto cache: *caches) {
                    auto urlcache = runUrlCheckOnCache(cache);
                    if ( urlcache && !urlcache->empty()) {
                        for ( auto url: *runUrlCheckOnCache(cache) ) {
                            if (WishLog::is_accepted_url(url)) detected_urls->append(url);
                        }
                    }
                }
                qfsw = std::make_shared<QFileSystemWatcher>();
                for (auto cache: *caches) {
                    qfsw->addPath(cache->filesystemFileName().c_str());
                }
                QApplication::connect(
                    qfsw.get(), &QFileSystemWatcher::fileChanged,
                    [&](QString path) { // c++ reminder: `, inc` in the lambda spec would copy inc.
                        auto checks = runUrlCheckOnCache(std::make_shared<QFile>(path));
                        if (checks) {
                            for (auto url: *checks) {
                                if (WishLog::is_accepted_url(url)) {
                                    if (!detected_urls->contains(url)) {
                                        detected_urls->append(url);
                                        if (given_option_auto_open_wishlog->isChecked())
                                            QDesktopServices::openUrl(url.append("#/log"));
                                    }
                                }
                            }
                        }
                    }
                );
            }
            break;
        }
        default: break;
        //case(ExeType::WutheringWaves):
    }
    std::cout << "integration set up" << std::endl;
}

std::unique_ptr<SARibbonPannel> LauncherCommand::get_panel_run() {
    /**
     * Always the Launcher, pretty much. Keep this out so I can refactor into game-dedicated panels
     *  with a background switch
     **/
    enlist_launch_action("Launcher", target_exec);
    for (auto file : *filtered_files) {
        if ( !target_exec.contains(file->filesystemFileName().filename().c_str()) ) {
            //auto inc = supported_games_impl.at(file->filesystemFileName().filename().c_str());
            auto inc = supported_games_impl.at(file->filesystemFileName().filename().c_str());
            if (first_game_detected == QAGL::QAGL_Game::UNKNOWN) {
                first_game_detected = convert_exetype(inc.get_exetype());
                // TODO: refactor fswatcher to run *when launching the target game*
                create_fs_integration(inc.get_exetype(), file);
            }
            enlist_launch_action(inc.get_label(),QFileInfo(*file).absoluteFilePath());
        }
    }
    std::unique_ptr<SARibbonPannel> panel_run = std::make_unique<SARibbonPannel>("Run game");
    for (std::shared_ptr<QAction> action: actions_execs) {
        panel_run->addLargeAction(action.get());
    }
    return std::move(panel_run);
}

// Does nothing yet.
std::unique_ptr<SARibbonPannel> LauncherCommand::get_panel_wishes() {
    DWishes wishes = DWishes(nullptr);
    wishes.show();
    return nullptr;
    /**
     * First, get the data_2 files.
     * Then, ask the user to select which one to look into.
     * Then, get all the URLs from it.
     * Then, show either the most recent one, or a list with a copy button on the right.
     **/
    enlist_launch_action("Launcher", target_exec);
    for (auto file : *filtered_files) {
        if ( !target_exec.contains(file->filesystemFileName().filename().c_str()) ) {
            auto game =supported_games_impl.at(file->filesystemFileName().filename().c_str());
            enlist_launch_action( game.get_label(), QFileInfo(*file).absoluteFilePath());
        }
    }
    std::unique_ptr<SARibbonPannel> panel_run = std::make_unique<SARibbonPannel>("Run game");
    for (std::shared_ptr<QAction> action: actions_execs) {
        panel_run->addLargeAction(action.get());
    }
    return std::move(panel_run);
}

std::unique_ptr<SARibbonPannel> LauncherCommand::get_panel_game() {
    given_action_game = std::make_unique<QAction>("Select Launch Executable");
    given->connect(
        given_action_game.get(),
        &QAction::triggered,
        [&](bool) {
            target_exec = QFileDialog::getOpenFileName(
                nullptr,
                "Get me the genshin",
                QString(std::getenv("STEAM_COMPAT_DATA_PATH")), "*.exe"
            );
            show_wishes_getter();
        }
    );

    std::unique_ptr<SARibbonPannel> panel_game = std::make_unique<SARibbonPannel>("Game Runtime");
    panel_game->addLargeAction(given_action_game.get());

    return std::move(panel_game);
}

std::shared_ptr<SARibbonCategory> LauncherCommand::getSocialsCat() {
    if (!given_panel_socials)
        given_panel_socials = std::move(get_panel_socials());

    if (!socials_cat) {
        socials_cat = std::make_shared<SARibbonCategory>();
        socials_cat->setCategoryName("Socials");
        socials_cat->setObjectName("socials_integration");

        socials_cat->addPannel(given_panel_socials.get());
    }
    return socials_cat;
}

void LauncherCommand::show_wishes_getter() {}

std::shared_ptr<SARibbonCategory> LauncherCommand::getLauncherCat() {
    if (!given_panel_game)
        given_panel_game = std::move(get_panel_game());
    if (!given_panel_proton)
        given_panel_proton = std::move(get_panel_proton());
    if (!given_panel_options)
        given_panel_options = std::move(get_panel_options());
    if (!given_panel_run)
        given_panel_run = std::move(get_panel_run());
    if (!given_panel_wishes)
        given_panel_wishes = std::move(get_panel_wishes());

    if (!given_cat) {
        given_cat = std::make_shared<SARibbonCategory>();
        given_cat->setCategoryName("Game");
        given_cat->setObjectName("gamedata");

        if (exec_provided)
            show_wishes_getter();
        else
            given_cat->addPannel(given_panel_game.get()); // launch sig?
        given_cat->addPannel(given_panel_proton.get());
        given_cat->addPannel(given_panel_options.get());
        given_cat->addPannel(given_panel_run.get());
    }
    return given_cat;
}

void LauncherCommand::setupRibbonWindow() {
    // Cool thing?
    given->setRibbonTheme(SARibbonTheme::RibbonThemeDark2);
    given->ribbonBar()->setRibbonStyle(SARibbonBar::RibbonStyleCompactThreeRow);
    given->ribbonBar()->setMinimumMode(true);
    given->ribbonBar()->setTabOnTitle(true);
    given->ribbonBar()->setApplicationButton(nullptr);
    given->ribbonBar()->addCategoryPage(getLauncherCat().get());
    given->ribbonBar()->addCategoryPage(getSocialsCat().get());

    given->windowButtonBar()->setupMaximizeButton(false);
}

void LauncherCommand::launcher() {
    if (!data) data = SettingsData::getSettingsData(); // todo: refresh
    if (!landing) {
        given = std::make_shared<SARibbonMainWindow>();

        given->window()->setAttribute(Qt::WA_TranslucentBackground);
        given->ribbonBar()->setStyleSheet("QMenuBar { border-top-left-radius:20px; border-top-right-radius:20px; }");
        given->windowButtonBar()->closeButton()->setStyleSheet("QToolButton {border-top-right-radius:20px;};");

        setupRibbonWindow();
        landing = std::make_unique<QAGL::Landing>(
            *qishes_launcher,
            std::move(data),
            QAGL::QAGL_App_Style::Normal,
            first_game_detected,     // TODO: autoselect "only exe detected"
            QAGL::QAGL_Region::global, // Global
            given
        );

        landing->hint_titlebar_height(given->ribbonBar()->titleBarHeight());

    }
    this->discord_report("qishes on main");

    landing->setOfflineMode(this->command_offline);

    landing->show(*qishes_launcher);
}

void LauncherCommand::command_create_application(int& argc, char **argv) {
    filtered_files = std::make_shared<std::list<std::shared_ptr<QFile>>>();
    // Quirk: Early detection of Steam Startup environment
    if (auto clientlaunch = std::getenv("SteamClientLaunch") ;
        std::getenv("SteamUser") &&
            (! clientlaunch || strcmp(clientlaunch, "1") != 0)
    ) {
        exit(0);
    }

    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings); // QWindowKit
    qishes_launcher = std::make_shared<QApplication>(argc, argv);
    QApplication::setApplicationName(APPNAME_GEN(.launcher));
    QApplication::setApplicationVersion(APP_VERSION);

    if (qishes_launcher->arguments().size() > 2) {
        // we can assume we have a 3rd argument. Use that as the execution target.
        if (qishes_launcher->arguments().at(2).endsWith("exe") ) {
            // all right we have an exe
            target_exec = qishes_launcher->arguments().at(2);
            exec_provided = true;
            if (steam_integration::get_steam_integration_instance()->is_steam_env()) {
                // TODO: fix getFiles, it REALLY ain't seeking right
                for (std::shared_ptr<QFile> file : *gachafs::getFiles(
                        "**/*.exe",
                        QString::fromStdString(wine::resolve_executable_path(target_exec.toStdString())),
                        true
                    )
                ) {
                    if (supported_games_impl.contains(file->filesystemFileName().filename())) {
                        filtered_files->push_back(file);
                    }
                }
            }
        }
    }

    (icon = std::make_shared<QPixmap>())
        ->loadFromData(QByteArray::fromBase64(qiqi_smol.toLocal8Bit(), QByteArray::Base64Encoding));

    QApplication::connect(
        qishes_launcher.get(), &QApplication::aboutToQuit,
        [&]() {

            icon.reset();
            action_exit.reset();
            action_launch.reset();

            actions_execs.clear();
            filtered_files->clear();
            filtered_files.reset();

            given_option_cloudpc.reset();
            given_option_deckenv.reset();
            given_option_obsvk.reset();
            given_option_mangohud.reset();
            given_option_discord.reset();
            given_option_auto_open_wishlog.reset();

            // Panel yeets
            if (given_proton_combo) given_proton_combo.reset();
            remove_panel_and_action(socials_cat, std::move(given_panel_socials), nullptr);
            remove_panel_and_action(given_cat, std::move(given_panel_proton), nullptr);
            remove_panel_and_action(given_cat, std::move(given_panel_game), std::move(given_action_game));
            remove_panel_and_action(given_cat, std::move(given_panel_run), std::move(given_action_run));
            remove_panel_and_action(given_cat, std::move(given_panel_options), nullptr);

            // Ribbon reset
            given_cat.reset();
            socials_cat.reset();

            // Landing window yeet
            landing.reset();
            tray.reset();
            tray_menu.reset();

            // last call
            given.reset();
        }
    );
}

void LauncherCommand::remove_panel_and_action(
    std::shared_ptr<SARibbonCategory> cat, std::unique_ptr<SARibbonPannel> panel, std::unique_ptr<QAction> action
) {
    if (cat && panel) {
        cat->removePannel(panel.get());
        if (action) panel->removeAction(action.get());
        panel.reset();
    }
}

void LauncherCommand::command_setup_parser() {
    parser->addPositionalArgument(
        "command",
        L18N_M("Command to run. MUST be launcher.")
    );

    // Offline mode is essentially "don't try to download metadata from the launcher upstream"
    //  right now this is only Hoyoverse's Genshin Impact, but it is intended to expand to multiple
    //  launcher arts by way of "guessed launcher profiles"
    parser->addOption(
        *(offline = std::make_shared<QCommandLineOption>(
            QStringList() << "o" << "offline",
            L18N("Enable Offline Mode.")
        ))
    );

    parser->addOption(
        *(steam = std::make_shared<QCommandLineOption>(
            QStringList() << "s" << "steam",
            L18N("Enable Steam Mode.")
        ))
    );

}

void LauncherCommand::command_process_parser() {
    parser->process(*qishes_launcher);
    if( parser->positionalArguments().empty() ||
        parser->positionalArguments()[0].compare(CommandSpecifier, Qt::CaseInsensitive) != 0
    ) {
        parser->showHelp(0);
    }
    this->command_steam =          parser->isSet(*steam);          // if set, always true
    this->command_offline =        parser->isSet(*offline);        // if set, always true
}

void LauncherCommand::discord_report(QString message) {
    if (given_option_discord && given_option_discord->isChecked()) {
        Discord::get_instance()->report_presence_message(message);
    }
}

int LauncherCommand::command_run() {
    this->discord_report("qishes loading");
    vlvproton::getInstance()->identify_installs();

    generate_tray_icon()->show();

    launcher();

    return qishes_launcher->exec();
}

std::shared_ptr<QAction> LauncherCommand::get_action_launcher_test() {
    action_launch = std::make_shared<QAction>();

    action_launch->setText("Open Launcher");
    QObject::connect(
        action_launch.get(), &QAction::triggered, // Signal
        [&](bool) { launcher(); }
    );

    return action_launch;
}

std::shared_ptr<QMenu> LauncherCommand::generate_menu() {
    tray_menu = std::make_shared<QMenu>();
    tray_menu->addActions( { get_action_launcher_test().get(), get_action_exit().get() } );
    return tray_menu;
}

std::shared_ptr<QSystemTrayIcon> LauncherCommand::generate_tray_icon() {
    tray = std::make_shared<QSystemTrayIcon>();
    tray->setContextMenu(generate_menu().get());
    tray->setIcon(*icon);
    QObject::connect(
        tray.get(), &QSystemTrayIcon::activated,
        [&](QSystemTrayIcon::ActivationReason) { tray->contextMenu()->show(); }
    );

    return tray;
}
