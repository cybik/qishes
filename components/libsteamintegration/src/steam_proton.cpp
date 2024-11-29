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

std::shared_ptr<steam_proton> steam_proton::mInstance = nullptr;

std::shared_ptr<steam_proton> steam_proton::getInstance() {
    if (!mInstance)
        mInstance = std::shared_ptr<steam_proton>(new steam_proton());
    return mInstance;
}

steam_proton::steam_proton() {
    mProton = vlvproton::getInstance(SteamEnvironment::get_steam_environment_instance()->getSteamBaseFolder());
    mArguments = std::make_shared<QStringList>();
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
        abort();
    }
}

void steam_proton::try_run(
    const std::string& target_executable,
    const std::list<std::string>& arguments,
    std::map<std::string, std::string> env_overrides
) {

    // Process init
    mProcess = std::make_shared<QProcess>();

    // Environment init
    for (auto [key, value] : env_overrides)
        mProcessEnvironment->insert(QString(key.c_str()), QString(value.c_str()));
    mProcess->setProcessEnvironment(*mProcessEnvironment);

    // What we cookin'
    // TODO: execution decorator
    mProcess->setProgram(mProton->get_selected_proton()->exec().c_str()); // proton
    mArguments->append("waitforexitandrun"); // always this
    mArguments->append(target_executable.c_str());

    // Arguments carry
    for (auto arg: arguments) (*mArguments) << arg.c_str(); // ah, standards conversion.
    mProcess->setArguments(*mArguments);
    qint64 mikkiku;
    mProcess->startDetached(&mikkiku);
    mProcess->waitForStarted();
    std::cout << QString::number(mikkiku).toStdString() << std::endl;
}
