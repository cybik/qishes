//
// Created by cybik on 22-07-31.
//

#ifndef QAGL_SETTINGS_H
#define QAGL_SETTINGS_H

#include <string>
#include <yaml-cpp/yaml.h>

class SettingsParsing {
public:
    virtual                             ~SettingsParsing() = default;
    virtual void                        parse(const YAML::Node& file) = 0;
    virtual std::unique_ptr<YAML::Node> generate() = 0;
    virtual void                        update() = 0;
protected:
    SettingsParsing() = default;
};


class Settings final : protected SettingsParsing {
public:
    Settings() = default; // TODO: config being initialized.
    explicit                    Settings(const YAML::Node& file);
    void                        parse(const YAML::Node& file) override;
    std::unique_ptr<YAML::Node> generate() override;
    std::string                 runner; // enum?
    std::string                 hud;    // enum?
    bool                        gamemode;
    void*                       env;          // map
    bool                        use_terminal;
    std::string                 to_string();

    void update() override;
};

#endif //QAGL_SETTINGS_H
