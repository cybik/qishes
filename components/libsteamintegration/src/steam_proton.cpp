/******************************************************************
 *
 * steam_proton.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-11-29, for qgachawishes.
 *
 ******************************************************************/

#include <iostream>
#include <steam_proton.h>
#include <vlvproton.h>

#include <steam_environment.h>

#include <QString>
#include <termcolor/termcolor.hpp>
#include <cctype>

std::shared_ptr<steam_proton> steam_proton::mInstance = nullptr;

std::shared_ptr<steam_proton> steam_proton::getInstance() {
    if (!mInstance)
        mInstance = std::shared_ptr<steam_proton>(new steam_proton());
    return mInstance;
}

steam_proton::steam_proton() {
    mProton = vlvproton::getInstance(
        SteamEnvironment::get_steam_environment_instance()->get_all_library_folders()
        //{SteamEnvironment::get_steam_environment_instance()->getSteamBaseFolder()}
    );
    //mArguments = std::make_shared<QStringList>();
    mProcessEnvironment = std::make_shared<QProcessEnvironment>(QProcessEnvironment::systemEnvironment());
    try_setup();
}

void steam_proton::select(const std::string& key) {
    mProton->select(key);
}

void steam_proton::try_setup() {
    if (!std::getenv("STEAM_COMPAT_APP_ID")) {
        mProcessEnvironment->insert("STEAM_COMPAT_APP_ID", "0");
    }
    for (auto base_folder: { "STEAM_BASE_FOLDER", "STEAM_COMPAT_CLIENT_INSTALL_PATH" })
    if (!std::getenv(base_folder)) {
        mProcessEnvironment->insert(
            base_folder,
            QString((std::filesystem::path(std::getenv("HOME")) / ".steam/root").c_str())
        );
    }
    if (!std::getenv("STEAM_COMPAT_DATA_PATH")) {
        // TODO: file explorer selector for now.
        //abort();
    }
}

std::string steam_proton::get_compat_dir_path() {
    return std::getenv("STEAM_COMPAT_DATA_PATH");
}

std::string steam_proton::get_compat_c_drive() {
    return get_compat_dir_path().append("/pfx/drive_c");
}

// TODO: pwd/cwd to eval the ini properly
void steam_proton::try_run(
    const std::string& target_executable,
    const Workaround::Handler workaround,
    const std::list<std::string>& arguments,
    const std::map<std::string, std::string>& env_overrides,
    const QString prefix
) {
    QStringList lArguments = QStringList();

    // instant decorate
    std::string true_target_executable = target_executable;
    std::unique_ptr<AWorkaround> workaround_handler = Workaround::getWorkaround(workaround, target_executable);

    // Always run
    workaround_handler->obtain(get_compat_c_drive());

    std::vector<std::string> decorated_executable = workaround_handler->decorate();

    // Process init
    mProcess = std::make_shared<QProcess>();

    // Environment init
    for (auto [key, value] : env_overrides)
        mProcessEnvironment->insert(QString(key.c_str()), QString(value.c_str()));
    mProcess->setProcessEnvironment(*mProcessEnvironment);

    // What we cookin'
    if (!prefix.isEmpty()) {
        lArguments.append(mProton->get_selected_proton()->exec().c_str());
    }

    mProcess->setProgram(
        prefix.isEmpty()
            ? mProton->get_selected_proton()->exec().c_str()
            : prefix
    ); // proton

    lArguments.append("run"); // always this
    if (decorated_executable.empty()) {
        lArguments.append(target_executable.c_str());
    } else {
        for (auto el: decorated_executable) lArguments.append(el.c_str());
    }
    QString cwd = "";
    if (target_executable.substr(1,2) == ":\\" ) {
        std::string calc_path = target_executable;
        std::ranges::replace(calc_path, '\\', '/');
        cwd = QString::fromStdString (
            get_compat_dir_path().append("/pfx/drive_")
                + static_cast<char>(std::tolower(calc_path.at(0)))
                + "/" + calc_path.substr(3)
            );
        cwd = cwd.sliced(0, cwd.lastIndexOf('/'));
        std::cout << "Using windows-style guesswork :: " << cwd.toStdString() << std::endl;
    } else {
        // unix style. Don't set working dir for now since idgaf yet
        cwd = QString::fromStdString(target_executable.substr(0, target_executable.find_last_of("/")));
        std::cout << "Using unix-style guesswork :: " << cwd.toStdString() << std::endl;
    }
    if (target_executable.starts_with("reg")) {
        cwd = ""; // Special case: reg workaround.
    }
    std::cout
        << termcolor::on_bright_green
            << "Attempting to launch " << target_executable  << std::endl
            << "\t" << "in prefix " << get_compat_dir_path() << std::endl
            << (cwd.isEmpty()? ""  : "\tusing working dir ")
            << (cwd.isEmpty()?"":cwd.toStdString())
        << termcolor::reset << std::endl;

    if (!cwd.isEmpty()) mProcess->setWorkingDirectory(cwd);
    // Arguments carry
    for (auto arg: arguments) lArguments << arg.c_str(); // ah, standards conversion.
    mProcess->setArguments(lArguments);
    qint64 mikkiku;
    mProcess->startDetached(&mikkiku);
    mProcess->waitForStarted();
    // TODO: UI element saying "this is running"
    // TODO: file watch on data_2 to auto-seek any new history URL
    // TODO: URL chooser with consult history recording
    // TODO: Consult history "mark all as consulted"
}
