//
// Created by cybik on 22-07-31.
//

#include <data/Settings.h>
#include <data/remote/runners/Wine.h>
#include <data/remote/runners/Proton.h>

#include <iostream>

#include <variant>

bool getBoolFromNode(const YAML::Node &file, const char* key) {
    return file[key].IsDefined() && file[key].as<bool>();
}

int getIntFromNode(const YAML::Node &file, const char* key) {
    return file[key].IsDefined() ? file[key].as<int>() : -1;
}

std::string getStringFromNode(const YAML::Node &file, const char* key) {
    return file[key].IsDefined() ? file[key].as<std::string>() : "";
}

void Settings::parse(const YAML::Node &file) {
    runner = getStringFromNode(file, "runner");
    dxvk = getStringFromNode(file, "dxvk");
    theme = getStringFromNode(file, "theme");
    hud = getStringFromNode(file, "hud");
    shaders = getStringFromNode(file, "shaders");
    //discord.parse(file["discord"]);
    gamemode = getBoolFromNode(file, "gamemode");
    //fps_unlocker = getBoolFromNode(file, "fps_unlocker");
    purge_dxvk_logs = getBoolFromNode(file, "purge_dxvk_logs");
    lang.parse(file["lang"]);
    env = nullptr;
    folders.parse(file["folders"]);
    purge_logs.parse(file["purge_logs"]);
    wine.parse(file["wine"]);
    borderless_window = getBoolFromNode(file, "borderless_window");
    use_terminal = getBoolFromNode(file, "use_terminal");
    skip_analytics = getBoolFromNode(file, "skip_analytics");
    server = getStringFromNode(file, "server");
}

Settings::Settings(const YAML::Node &file) {
    parse(file);
    Proton::get_instance();
    Wine::get_instance();
}

std::string Settings::to_string() {
    return YAML::Dump(*generate());
}

std::unique_ptr<YAML::Node> Settings::generate() {
    auto out = std::make_unique<YAML::Node>();
    (*out)["runner"] = runner;
    (*out)["dxvk"] = dxvk;
    (*out)["theme"] = theme;
    (*out)["hud"] = hud;
    (*out)["shaders"] = shaders;
    //(*out)["discord"] = (*discord.generate());
    (*out)["gamemode"] = gamemode;
    //(*out)["fps_unlocker"] = fps_unlocker;
    (*out)["purge_dxvk_logs"] = purge_dxvk_logs;
    (*out)["lang"] = (*lang.generate());
    (*out)["env"] = std::list<std::string>();
    (*out)["folders"] = (*folders.generate());
    (*out)["purge_logs"] = (*purge_logs.generate());
    (*out)["wine"] = (*wine.generate());
    (*out)["borderless_window"] = borderless_window;
    (*out)["use_terminal"] = use_terminal;
    (*out)["skip_analytics"] = skip_analytics;
    (*out)["server"] = server;
    return std::move(out);
}

void Settings::update() {
    lang.update();
    folders.update();
    purge_logs.update();
    wine.update();
    //discord.update();
    // do updates here
}

void SettingsWine::parse(const YAML::Node &file) {
    sync = getStringFromNode(file, "sync");
    fsr = getBoolFromNode(file, "fsr");
    virtual_desktop.parse(file["virtual_desktop"]);
}

std::unique_ptr<YAML::Node> SettingsWine::generate() {
    auto out = std::make_unique<YAML::Node>();
    (*out)["sync"] = sync;
    (*out)["fsr"] = fsr;
    (*out)["virtual_desktop"] = (*virtual_desktop.generate());
    return std::move(out);
}

void SettingsWine::update() {

}

void SettingsWineVirtualDesktop::parse(const YAML::Node &file) {
    enabled = getBoolFromNode(file, "enabled");
    width = getIntFromNode(file, "width");
    height = getIntFromNode(file, "height");
}

std::unique_ptr<YAML::Node> SettingsWineVirtualDesktop::generate() {
    auto out = std::make_unique<YAML::Node>();
    (*out)["enabled"] = enabled;
    (*out)["width"] = width;
    (*out)["height"] = height;
    return std::move(out);
}

void SettingsWineVirtualDesktop::update() {

}

void SettingsPurge::parse(const YAML::Node &file) {
    launcher = getStringFromNode(file, "launcher");
    game = getBoolFromNode(file, "game");
}

std::unique_ptr<YAML::Node> SettingsPurge::generate() {
    auto out = std::make_unique<YAML::Node>();
    (*out)["game"] = game;
    (*out)["launcher"] = launcher;
    return std::move(out);
}

void SettingsPurge::update() {

}

void SettingsFolders::parse(const YAML::Node &file) {
    prefix = getStringFromNode(file, "prefix");
    game = getStringFromNode(file, "game");
    temp = getStringFromNode(file, "temp");
}

std::unique_ptr<YAML::Node> SettingsFolders::generate() {
    auto out = std::make_unique<YAML::Node>();
    (*out)["prefix"] = prefix;
    (*out)["game"] = game;
    (*out)["temp"] = temp;
    return std::move(out);
}

void SettingsFolders::update() {

}

void SettingsLanguage::parse(const YAML::Node &file) {
    launcher = getStringFromNode(file, "launcher");
    if(file["voice"].Type() != YAML::NodeType::Sequence) {
        abort();
    }
    for(int i = 0; i < file["voice"].size(); i++) {
        //voice.push_back( file["voice"][i].as<std::string>());
        if(file["voice"][i].as<std::string>() == "en-us") lang_game_en = true;
        else if(file["voice"][i].as<std::string>() == "ja-jp") lang_game_jp = true;
        else if(file["voice"][i].as<std::string>() == "ko-kr") lang_game_kr = true;
        else if(file["voice"][i].as<std::string>() == "zh-cn") lang_game_zh = true;
    }
}

std::unique_ptr<YAML::Node> SettingsLanguage::generate() {
    auto out = std::make_unique<YAML::Node>();
    (*out)["launcher"] = launcher;

    if(lang_game_en) (*out)["voice"].push_back("en-us");
    if(lang_game_jp) (*out)["voice"].push_back("ja-jp");
    if(lang_game_kr) (*out)["voice"].push_back("ko-kr");
    if(lang_game_zh) (*out)["voice"].push_back("zh-cn");

    return std::move(out);
}

void SettingsLanguage::update() {

}

/*
void SettingsDiscord::parse(const YAML::Node &file) {
    enabled = { getBoolFromNode(file, "enabled") , std::weak_ptr<SettingsCheckbox>() };
    timer = getBoolFromNode(file, "timer");
    inLauncher.parse(file["states"]["in-launcher"]);
    inGame.parse(file["states"]["in-game"]);
}

std::unique_ptr<YAML::Node> SettingsDiscord::generate() {
    auto out = std::make_unique<YAML::Node>();
    (*out)["enabled"] = std::get<bool>(enabled);
    (*out)["timer"] = timer;
    YAML::Node states;
    states["in-launcher"] = (*inLauncher.generate());
    states["in-game"] = (*inGame.generate());
    (*out)["states"] = states;
    return std::move(out);
}

void SettingsDiscord::update() {
    if(std::get<std::weak_ptr<SettingsCheckbox>>(enabled).lock() != nullptr) {
        std::get<bool>(enabled) = (std::get<std::weak_ptr<SettingsCheckbox>>(enabled).lock()->isChecked());
    }
}

void SettingsDiscordState::parse(const YAML::Node &file) {
    details = getStringFromNode(file, "details");
    state = getStringFromNode(file, "state");
    icon = getStringFromNode(file, "icon");
}

std::unique_ptr<YAML::Node> SettingsDiscordState::generate() {
    auto out = std::make_unique<YAML::Node>();
    (*out)["details"] = details;
    (*out)["state"] = state;
    (*out)["icon"] = icon;
    return std::move(out);
}

void SettingsDiscordState::update() {

}
*/
