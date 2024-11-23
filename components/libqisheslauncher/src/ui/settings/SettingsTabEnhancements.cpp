//
// Created by cybik on 22-07-24.
//

#include "SettingsTabEnhancements.h"

SettingsTabEnhancements::SettingsTabEnhancements(Nedrysoft::Ribbon::RibbonWidget* ri) : SettingsTab() {
    setupWINEGroup();
    setupGameGroup();
    setupLauncherGroup();

    addTab(ri, "Enhancements");
}

void SettingsTabEnhancements::setupWINEGroup() {
    _enhWine = std::make_shared<SettingsGroup>("WINE");
    setupHudCombo(_enhWine);
    setupWineSyncCombo(_enhWine);
    setupAmdFSRCb(_enhWine);
    setupVirtualDesktopCb(_enhWine);

    _tabLayout->addWidget(_enhWine->getWidget());
}

void SettingsTabEnhancements::setupHudCombo(std::shared_ptr<SettingsGroup> group) {
    _cbHud = std::make_shared<SettingsCombo>("HUD");
    _cbHud->addChoices(3, "None", "DXVK", "MangoHUD");
    group->addControl(_cbHud);
}

void SettingsTabEnhancements::setupWineSyncCombo(std::shared_ptr<SettingsGroup> group) {
    _cbWineSync = std::make_shared<SettingsCombo>("WINE Synchronization");
    _cbWineSync->addChoices(4, "None", "ESync", "FSync", "Futex2");
    group->addControl(_cbWineSync);
}

void SettingsTabEnhancements::setupAmdFSRCb(std::shared_ptr<SettingsGroup> group) {
    _cbFSR = std::make_shared<SettingsCheckbox>("Allow AMD FSR");
    group->addControl(_cbFSR);
}

void SettingsTabEnhancements::setupVirtualDesktopCb(std::shared_ptr<SettingsGroup> group) {
    _cbVirDesk = std::make_shared<SettingsCheckbox>("Virtual Desktop");
    group->addControl(_cbVirDesk);
}

///

void SettingsTabEnhancements::setupGameGroup() {
    _enhGame = std::make_shared<SettingsGroup>("Game");
    setupGameModeCb(_enhGame);
    setupBorderlessWindowCb(_enhGame);
    //setupUnlocFPSCb(_enhGame);
    setupTerminalCb(_enhGame);
    _tabLayout->addWidget(_enhGame->getWidget());
}

void SettingsTabEnhancements::setupGameModeCb(std::shared_ptr<SettingsGroup> group) {
    _cbGamemode = std::make_shared<SettingsCheckbox>("Use GameMode");
    group->addControl(_cbGamemode);
}

void SettingsTabEnhancements::setupBorderlessWindowCb(std::shared_ptr<SettingsGroup> group) {
    _cbWindow = std::make_shared<SettingsCheckbox>("Allow Borderless Window");
    group->addControl(_cbWindow);
}

/*
void SettingsTabEnhancements::setupUnlocFPSCb(std::shared_ptr<SettingsGroup> group) {
    _cbFPS = std::make_shared<SettingsCheckbox>("Unlock FPS (DANGER)");
    group->addControl(_cbFPS);
}
*/

void SettingsTabEnhancements::setupTerminalCb(std::shared_ptr<SettingsGroup> group) {
    _cbTerm = std::make_shared<SettingsCheckbox>("Use Terminal");
    group->addControl(_cbTerm);
}

void SettingsTabEnhancements::setupLauncherGroup() {
    _enhLauncher = std::make_shared<SettingsGroup>("Launcher");
    setupLauncherCombo(_enhLauncher);
    _tabLayout->addWidget(_enhLauncher->getWidget());
}

void SettingsTabEnhancements::setupLauncherCombo(std::shared_ptr<SettingsGroup> group) {
    _cbLauncherCombo = std::make_shared<SettingsCombo>("Delete launcher logs");
    _cbLauncherCombo->addChoices(3,
                       "1 day", "3 days", "5 days"
    );
    group->addControl(_cbLauncherCombo);
}

void SettingsTabEnhancements::parse(std::shared_ptr<Settings> data) {

}