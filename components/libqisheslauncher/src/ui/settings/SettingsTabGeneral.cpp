//
// Created by cybik on 22-07-24.
//

#include "SettingsTabGeneral.h"

#include <algorithm>
#include <QMessageBox>

SettingsTabGeneral::SettingsTabGeneral(Nedrysoft::Ribbon::RibbonWidget* ri) : SettingsTab() {
    setupGeneralGroup();
    setupPatchInfoGroup();
    setupPatchOpsGroup();
    setupDiscordGroup();
    setupDiscordTextOptionsGroup();
    setupDiscordIconOptionsGroup();

    setupGameLaunch();
    addTab(ri, "General");
}

void SettingsTabGeneral::setupGeneralGroup() {
    _general = std::make_shared<SettingsGroup>("Launcher");

    setupLanguage(_general);
    setupTheme(_general);

    _tabLayout->addWidget(_general->getWidget());
}


void SettingsTabGeneral::setupLanguage(std::shared_ptr<SettingsGroup> group) {
    _cbLang = std::make_shared<SettingsCombo>("Language");
    _cbLang->addChoices(11,
                        "English (US)", "Русский", "Español",
                        "Deutsch", "Français", "Italiano", "日本語",
                        "Magyar", "Norsk", "简体中文", "Engwish"
    );
    group->addControl(_cbLang);
}

void SettingsTabGeneral::setupTheme(std::shared_ptr<SettingsGroup> group) {
    _cbTheme = std::make_shared<SettingsCombo>("Theme");
    _cbTheme->addChoices(3, "System", "Dark", "Light");
    group->addControl(_cbTheme);
}

void SettingsTabGeneral::setupPatchInfoGroup() {
    _patch = std::make_shared<SettingsGroup>("Patch Info");

    setupPatchDesc(_patch);
    setupPatchInfo(_patch);

    _tabLayout->addWidget(_patch->getWidget());
}

void SettingsTabGeneral::setupGameLaunch() {
    _launch = std::make_shared<SettingsGroup>("Launch it");
    setupLaunchDesc(_launch);
    _tabLayout->addWidget(_launch->getWidget());
}

void SettingsTabGeneral::onLaunchClick() {
    QMessageBox * msg = new QMessageBox();
    msg->setText("mikkiku");
    msg->standardButtons();
    msg->show();

    _launcher = Launch::getLaunch();
    _launcher->LaunchIt(&_pid);
}

void SettingsTabGeneral::setupLaunchDesc(std::shared_ptr<SettingsGroup> group) {
    _cbLaunch = std::make_shared<SettingsButton>("YEET");
    _cbLaunch->addHandler( std::make_unique<std::function<void(bool)>>([&](bool) { this->onLaunchClick(); }) );
    group->addControl(_cbLaunch);
}

void SettingsTabGeneral::setupPatchOpsGroup() {
    _patchOps = std::make_shared<SettingsGroup>("Patch Ops");

    setupPatchRevert(_patchOps);
    setupPatchReapply(_patchOps);

    _tabLayout->addWidget(_patchOps->getWidget());
}

void SettingsTabGeneral::setupPatchDesc(std::shared_ptr<SettingsGroup> group) {
    _cbPatchDesc = std::make_shared<SettingsLabel>("Patch Version:");
    group->addControl(_cbPatchDesc);
}

void SettingsTabGeneral::setupPatchInfo(std::shared_ptr<SettingsGroup> group) {
    _cbPatchInfo = std::make_shared<SettingsLabel>("Patch Info");
    _cbPatchInfo->getControl()
                ->setAlignment(Qt::AlignmentFlag::AlignRight);
    _cbPatchInfo->setColor(QColor::fromCmyk(70,60,0,10));
    group->addControl(_cbPatchInfo);
}

void SettingsTabGeneral::setupPatchRevert(std::shared_ptr<SettingsGroup> group) {
    _cbPatchRevert = std::make_shared<SettingsButton>("Revert Patch");
    group->addControl(_cbPatchRevert);
}

void SettingsTabGeneral::setupPatchReapply(std::shared_ptr<SettingsGroup> group) {
    _cbPatchReapply = std::make_shared<SettingsButton>("Reapply Patch");
    group->addControl(_cbPatchReapply);
}


void SettingsTabGeneral::setupDiscordGroup() {
    _discord = std::make_shared<SettingsGroup>("Discord");

    setupDiscord(_discord);

    _tabLayout->addWidget(_discord->getWidget());
}

void SettingsTabGeneral::setupDiscord(std::shared_ptr<SettingsGroup> group) {
    _cbDiscordEnabled = std::make_shared<SettingsCheckbox>("Enable Discord RPC");
    group->addControl(_cbDiscordEnabled);
}

void SettingsTabGeneral::setupDiscordTextOptionsGroup() {
    _discordSettings = std::make_shared<SettingsGroup>("Discord");

    setupDiscordTextOptions(_discordSettings);
    _discordSettings->getWidget()->setVisible(false);

    _tabLayout->addWidget(_discordSettings->getWidget());
}

void SettingsTabGeneral::setupDiscordTextOptions(std::shared_ptr<SettingsGroup> group) {

}

void SettingsTabGeneral::setupDiscordIconOptionsGroup() {
    _discordIconSettings = std::make_shared<SettingsGroup>("Discord Icons");

    setupDiscordIconOptions(_discordIconSettings);
    _discordIconSettings->getWidget()->setVisible(false);

    _tabLayout->addWidget(_discordIconSettings->getWidget());
}

void SettingsTabGeneral::setupDiscordIconOptions(std::shared_ptr<SettingsGroup> group) {

}

void SettingsTabGeneral::parse(std::shared_ptr<Settings> data) {
    _cbDiscordEnabled->setChecked(std::get<bool>(data->discord.enabled));
    std::get<std::weak_ptr<SettingsCheckbox>>(data->discord.enabled) = _cbDiscordEnabled;
}