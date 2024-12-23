/******************************************************************
 *
 * LauncherCommand.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-07-10 for qgachawishes.
 *
 ******************************************************************/

#include <commands/LauncherCommand.h>

#include <QApplication>

#include <QObject>

#include <iostream>

#include <chrono>
#include <steam_integration.h>

#include <gachafs.h>

const QString LauncherCommand::CommandSpecifier = "launcher";

std::shared_ptr<SettingsData> LauncherCommand::data = nullptr;
std::unique_ptr<QAGL::Landing> LauncherCommand::landing = nullptr;

std::shared_ptr<QAction> LauncherCommand::get_action_exit() {
    action_exit = std::make_shared<QAction>();

    action_exit->setText("Quit");
    QObject::connect(
            action_exit.get(), &QAction::triggered, // Signal
            [&](bool) {
                std::cout << "mikkikue" << std::endl;
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
    given_option_mangohud = std::move(get_checkbox("MangoHUD", "cbMango", true));
    given_option_deckenv = std::move(get_checkbox("Fakeout Deck", "cbDeckMode", true));
    given_option_obsvk = std::move(get_checkbox("OBS VkCapture Mode", "cbVkCap", true));
    given_option_cloudpc = std::move(get_checkbox(
        "Cloud Masquerade", "cbImpersonateCloud", true)
    );

    std::unique_ptr<SARibbonPannel> panel_opt = std::make_unique<SARibbonPannel>();
    panel_opt->addSmallWidget(given_option_mangohud.get());
    panel_opt->addSmallWidget(given_option_deckenv.get());
    panel_opt->addSmallWidget(given_option_obsvk.get());
    panel_opt->addSmallWidget(given_option_cloudpc.get());
    panel_opt->setPannelName("Options");
    return std::move(panel_opt);
}

std::unique_ptr<SARibbonPannel> LauncherCommand::get_panel_proton() {
    given_proton_combo = std::make_unique<SARibbonComboBox>();
    given_proton_combo->setWindowTitle("ProtonSelect");
    given_proton_combo->setObjectName("ProtonSelect");
    for (auto str: vlvproton::getInstance()->get_available_protons()) {
        given_proton_combo->addItem(QString(str.c_str()));
    }
    // TODO: set current selected to match config that's not implemented yet
    std::unique_ptr<SARibbonPannel> panel_proton = std::make_unique<SARibbonPannel>();
    panel_proton->addSmallWidget(given_proton_combo.get());
    panel_proton->setPannelName("Proton Runtime");
    return std::move(panel_proton);
}

void LauncherCommand::run_the_magic(const QString& target_exe) {
    std::map<std::string, std::string> envs = {};
    std::list<std::string> arguments = {};
    if (given_option_mangohud->isChecked()) envs["MANGOHUD"] = "1";
    if (given_option_deckenv->isChecked())  envs["SteamDeck"] = "1";
    if (given_option_obsvk->isChecked())    envs["OBS_VKCAPTURE"] = "1";
    if (given_option_cloudpc->isChecked()) {
        arguments.emplace_back("-platform_type");
        arguments.emplace_back("CLOUD_THIRD_PARTY_PC");
    }
    steam_integration::get_steam_integration_instance()->proton()->try_run(
        target_exe.toStdString(), arguments, envs
    );
}

void LauncherCommand::enlist_launch_action(QString message, QString executable) {
    std::shared_ptr<QAction> action_run = std::make_unique<QAction>(message);
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

std::unique_ptr<SARibbonPannel> LauncherCommand::get_panel_run() {
    enlist_launch_action("Try to run", target_exec);
    for (auto file : *filtered_files) {
        std::cout << file->fileName().toStdString() << std::endl;
        if ( !target_exec.contains(file->filesystemFileName().filename().c_str()) ) {
            enlist_launch_action(
                QString(file->filesystemFileName().filename().c_str()),
                QFileInfo(*file).absoluteFilePath());
        }
    }
    std::unique_ptr<SARibbonPannel> panel_run = std::make_unique<SARibbonPannel>("Run game");
    //panel_run->addLargeAction(given_action_run.get());
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
        }
    );

    std::unique_ptr<SARibbonPannel> panel_game = std::make_unique<SARibbonPannel>("Game Runtime");
    panel_game->addLargeAction(given_action_game.get());

    return std::move(panel_game);
}

std::shared_ptr<SARibbonCategory> LauncherCommand::getLauncherCat() {
    if (!given_panel_game)
        given_panel_game = std::move(get_panel_game());
    if (!given_panel_proton)
        given_panel_proton = std::move(get_panel_proton());
    if (!given_panel_options)
        given_panel_options = std::move(get_panel_options());
    if (!given_panel_run)
        given_panel_run = std::move(get_panel_run());

    if (!given_cat) {
        given_cat = std::make_shared<SARibbonCategory>();
        given_cat->setCategoryName("Game");
        given_cat->setObjectName("gamedata");

        if (!exec_provided) given_cat->addPannel(given_panel_game.get());
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

    given->windowButtonBar()->setupMaximizeButton(false);
}

void LauncherCommand::launcher() {
    if (!data) data = SettingsData::getSettingsData(); // todo: refresh
    if (!landing) {
        given = std::make_shared<SARibbonMainWindow>();

        given->window()->setAttribute(Qt::WA_TranslucentBackground);
        given->ribbonBar()->setStyleSheet("QMenuBar { border-top-left-radius:20px; border-top-right-radius:20px; }");
        given->windowButtonBar()->closeButton()->setStyleSheet("QToolButton {border-top-right-radius:20px;};");

        landing = std::make_unique<QAGL::Landing>(
            *qishes_launcher,
            std::move(data),
            QAGL::QAGL_App_Style::Normal,
            QAGL::QAGL_Game::h4ke,     // Genshin
            QAGL::QAGL_Region::global, // Global
            given
        );

        setupRibbonWindow();
        landing->hint_titlebar_height(given->ribbonBar()->titleBarHeight());

    }
    dis->report_presence_message("qishes on main");

    landing->setOfflineMode(this->command_offline);

    landing->show(*qishes_launcher);
}

QString LauncherCommand::resolve_executable_path() {
    if (!target_exec.contains(":\\")) {
        return target_exec;
    }
    QString reparsed_exec_path = target_exec.split(":")[1];
    reparsed_exec_path = reparsed_exec_path.replace("\\", "/");
    reparsed_exec_path = reparsed_exec_path.replace("//", "/"); // dedupe
    reparsed_exec_path = (qgetenv("STEAM_COMPAT_DATA_PATH") + "/pfx/drive_c/" + reparsed_exec_path);
    reparsed_exec_path = reparsed_exec_path.replace("//", "/"); // dedupe
    reparsed_exec_path = reparsed_exec_path.first(reparsed_exec_path.lastIndexOf("/"));

    return reparsed_exec_path;
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
                for (std::shared_ptr<QFile> file : *gachafs::getFiles("**/*.exe", resolve_executable_path(), true)) {
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
            // Discord yeet
            dis.reset();

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

            // Panel yeets
            if (given_proton_combo) given_proton_combo.reset();
            remove_panel_and_action(given_cat, std::move(given_panel_proton), nullptr);
            remove_panel_and_action(given_cat, std::move(given_panel_game), std::move(given_action_game));
            remove_panel_and_action(given_cat, std::move(given_panel_run), std::move(given_action_run));
            remove_panel_and_action(given_cat, std::move(given_panel_options), nullptr);

            // Ribbon reset
            given_cat.reset();

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

}

void LauncherCommand::command_process_parser() {
    parser->process(*qishes_launcher);
    if( parser->positionalArguments().empty() ||
        parser->positionalArguments()[0].compare(CommandSpecifier, Qt::CaseInsensitive) != 0
    ) {
        parser->showHelp(0);
    }
    this->command_offline =        parser->isSet(*offline);        // if set, always true
}

int LauncherCommand::command_run() {
    dis = Discord::get_instance()->report_presence_message("qishes loading");
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
