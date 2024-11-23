//
// Created by cybik on 22-07-24.
//

#ifndef QAGL_SETTINGSTABGAME_H
#define QAGL_SETTINGSTABGAME_H


#include "SettingsTab.h"
#include "3rdParty/nedrysoft/Ribbon/include/RibbonWidget"
#include "3rdParty/nedrysoft/Ribbon/include/RibbonGroup"
#include <QWidget>
#include <QHBoxLayout>

#include "launcher/ui/settings/util/SettingsControls.h"
#include "launcher/ui/settings/util/SettingsGroup.h"


class SettingsTabGame : public SettingsTab {
public:
    explicit SettingsTabGame(Nedrysoft::Ribbon::RibbonWidget* ri);
    void parse(std::shared_ptr<Settings>) override;
private:
    std::shared_ptr<Nedrysoft::Ribbon::RibbonGroup> _setup;

    void setupGeneralGroup();
    std::shared_ptr<SettingsGroup> _general;
    void setupGeneral(std::shared_ptr<SettingsGroup>);

    void setupVoicePacksGroup();
    std::shared_ptr<SettingsGroup> _voicepacks;
    void setupVoicePacks(std::shared_ptr<SettingsGroup>);
    std::shared_ptr<SettingsCheckbox> _cbEN;
    std::shared_ptr<SettingsCheckbox> _cbJA;
    std::shared_ptr<SettingsCheckbox> _cbKR;
    std::shared_ptr<SettingsCheckbox> _cbZH;

    void setupMiscGroup();
    std::shared_ptr<SettingsGroup> _misc;
    void setupMisc(std::shared_ptr<SettingsGroup>);
};

#endif //QAGL_SETTINGSTABGAME_H
