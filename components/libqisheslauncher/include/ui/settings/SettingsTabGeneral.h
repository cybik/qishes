//
// Created by cybik on 22-07-24.
//

#ifndef QAGL_SETTINGSTABGENERAL_H
#define QAGL_SETTINGSTABGENERAL_H

#include "SettingsTab.h"
#include "3rdParty/nedrysoft/Ribbon/include/RibbonWidget"
#include <QWidget>
#include <QHBoxLayout>

#include "launcher/ui/settings/util/SettingsControls.h"
#include "launcher/ui/settings/util/SettingsGroup.h"
#include "launcher/process/launch.h"

class SettingsTabGeneral : public SettingsTab {
public:
    explicit SettingsTabGeneral(Nedrysoft::Ribbon::RibbonWidget* ri);
    void parse(std::shared_ptr<Settings> data) override;
private:
    void setupGeneralGroup();
    std::shared_ptr<SettingsGroup> _general;
    void setupLanguage(std::shared_ptr<SettingsGroup>);
    std::shared_ptr<SettingsCombo> _cbLang;
    void setupTheme(std::shared_ptr<SettingsGroup>);
    std::shared_ptr<SettingsCombo> _cbTheme;

    void setupGameLaunch();
    std::shared_ptr<SettingsGroup> _launch;
    void setupLaunchDesc(std::shared_ptr<SettingsGroup>);
    std::shared_ptr<SettingsButton> _cbLaunch;
    void onLaunchClick();

    void setupPatchInfoGroup();
    std::shared_ptr<SettingsGroup> _patch;
    void setupPatchDesc(std::shared_ptr<SettingsGroup>);
    std::shared_ptr<SettingsLabel> _cbPatchDesc;
    void setupPatchInfo(std::shared_ptr<SettingsGroup>);
    std::shared_ptr<SettingsLabel> _cbPatchInfo;
    void setupPatchOpsGroup();
    std::shared_ptr<SettingsGroup> _patchOps;
    void setupPatchRevert(std::shared_ptr<SettingsGroup>);
    std::shared_ptr<SettingsButton> _cbPatchRevert;
    void setupPatchReapply(std::shared_ptr<SettingsGroup>);
    std::shared_ptr<SettingsButton> _cbPatchReapply;

    void setupDiscordGroup();
    std::shared_ptr<SettingsGroup> _discord;
    void setupDiscord(std::shared_ptr<SettingsGroup>);
    std::shared_ptr<SettingsCheckbox> _cbDiscordEnabled;

    void setupDiscordTextOptionsGroup();
    std::shared_ptr<SettingsGroup> _discordSettings;
    void setupDiscordTextOptions(std::shared_ptr<SettingsGroup>);

    void setupDiscordIconOptionsGroup();
    std::shared_ptr<SettingsGroup> _discordIconSettings;
    void setupDiscordIconOptions(std::shared_ptr<SettingsGroup>);

    std::shared_ptr<Launch> _launcher;
    qint64 _pid;
};


#endif //QAGL_SETTINGSTABGENERAL_H
