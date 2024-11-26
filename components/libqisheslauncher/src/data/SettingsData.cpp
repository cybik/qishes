//
// Created by cybik on 22-07-30.
//

#include <data/SettingsData.h>

#include <string>

#include <yaml-cpp/node/emit.h>

#define ANIME_PATH "/moe.cybik.qishe"

#include <iostream>
#include <fstream>
#include <ios>
#include <QDir>
#include <QStandardPaths>

std::shared_ptr<SettingsData> SettingsData::_settingsData = nullptr;
std::shared_ptr<SettingsData> SettingsData::getSettingsData() {
    if(!_settingsData) {
        QDir config_dir = QDir(
            QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation)[0]
                + std::filesystem::path::preferred_separator
                + ANIME_PATH
        );
        if (!config_dir.exists()) create_directories(config_dir.filesystemPath());
        _settingsData = std::make_shared<SettingsData>(config_dir.filesystemPath());
    }
    return _settingsData;
}

std::shared_ptr<Settings> SettingsData::getSettings() {
    return _settings;
}

SettingsData::SettingsData(std::filesystem::path path) {
    _file = ((_path = path).string() + "/config.yaml");

    if(is_regular_file(_file)) {
        _settings = std::make_shared<Settings>(YAML::LoadFile(_file.string()));
        return;
    }
    //_settings = std::make_shared<Settings>();
}

void SettingsData::saveSettings() {
    /*std::cout << _file.string() << std::endl;
    std::cout << "------------------------" << std::endl;
    std::cout << _settings->to_string();
    std::cout << "------------------------" << std::endl;
    */
    std::ofstream ofs;
    ofs.open(_file, std::ios_base::out);
    ofs << _settings->to_string() << std::endl;
    ofs.close();
}

void SettingsData::update() {
    getSettings()->update();
}
