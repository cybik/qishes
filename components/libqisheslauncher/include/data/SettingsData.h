//
// Created by cybik on 22-07-30.
//

#ifndef QAGL_SETTINGSDATA_H
#define QAGL_SETTINGSDATA_H


#include <memory>
#include <filesystem>
#include <string>

#include "Settings.h"

class SettingsData {
public:
    static std::shared_ptr<SettingsData> getSettingsData();
    explicit SettingsData(std::filesystem::path);

    std::shared_ptr<Settings> getSettings();
    void update();
    void saveSettings();
private:
    SettingsData() = delete;

    std::filesystem::path _path;
    std::filesystem::path _file;

    std::shared_ptr<Settings> _settings;
    static std::shared_ptr<SettingsData> _settingsData;
};


#endif //QAGL_SETTINGSDATA_H
