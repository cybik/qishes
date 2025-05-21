//
// Created by cybik on 22-07-31.
//

#include <data/Settings.h>
#include <data/remote/runners/Wine.h>
#include <data/remote/runners/Proton.h>

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
    hud = getBoolFromNode(file, "hud");
    vkcap = getBoolFromNode(file, "vkcap");
    wayland = getBoolFromNode(file, "wayland");
    nowmdeco = getBoolFromNode(file, "nowmdeco");
    deckenv = getBoolFromNode(file, "deckenv");
    gamemode = getBoolFromNode(file, "gamemode");
    xalia = getBoolFromNode(file, "xalia");
    env = nullptr;
    use_terminal = getBoolFromNode(file, "use_terminal");
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
    auto out           = std::make_unique<YAML::Node>();
    (*out)["runner"]   = runner;
    (*out)["hud"]      = hud;
    (*out)["vkcap"]    = vkcap;
    (*out)["wayland"]  = wayland;
    (*out)["nowmdeco"] = nowmdeco;
    (*out)["deckenv"]  = deckenv;
    (*out)["xalia"]    = xalia;
    (*out)["gamemode"] = gamemode;
    (*out)["env"]      = std::list<std::string>();
    (*out)["use_terminal"] = use_terminal;
    return std::move(out);
}

void Settings::update() {
    // do updates here
    abort();
}

