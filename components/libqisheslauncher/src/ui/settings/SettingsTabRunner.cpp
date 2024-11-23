//
// Created by cybik on 22-07-24.
//

#include "SettingsTabRunner.h"

SettingsTabRunner::SettingsTabRunner(Nedrysoft::Ribbon::RibbonWidget* ri) : SettingsTab() {
    setupRunnerGroup();
    setupDXVKGroup();

    addTab(ri, "Runner");
}

void SettingsTabRunner::setupDXVKGroup() {
    _runDXVK = std::make_shared<SettingsGroup>("DXVK Setup");
    setupDXVKs(_runDXVK);
    _tabLayout->addWidget(_runDXVK->getWidget());
}

void SettingsTabRunner::setupDXVKs(std::shared_ptr<SettingsGroup> group) {
    // noop
}

void SettingsTabRunner::setupRunnerGroup() {
    _runWINE = std::make_shared<SettingsGroup>("Runner Setup");
    setupRunners(_runWINE);
    _tabLayout->addWidget(_runWINE->getWidget());
}

void SettingsTabRunner::setupRunners(std::shared_ptr<SettingsGroup> group) {
    _cbWINE = std::make_shared<SettingsCombo>("Runner Style");
    _cbWINE->addChoice(Proton::get_instance()->getRunnerName(), Proton::get_instance()->getRootKey());
    _cbWINE->addChoice(Wine::get_instance()->getRunnerName(), Wine::get_instance()->getRootKey());
    group->addControl(_cbWINE);
}

void SettingsTabRunner::parse(std::shared_ptr<Settings> data) {

}