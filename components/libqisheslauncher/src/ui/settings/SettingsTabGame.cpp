//
// Created by cybik on 22-07-24.
//

#include "SettingsTabGame.h"

SettingsTabGame::SettingsTabGame(Nedrysoft::Ribbon::RibbonWidget* ri) : SettingsTab() {
    setupGeneralGroup();
    setupVoicePacksGroup();
    setupMiscGroup();

    addTab(ri, "Game Setup");
}

void SettingsTabGame::setupGeneralGroup() {
    _general = std::make_shared<SettingsGroup>("General");

    setupGeneral(_general);
    _tabLayout->addWidget(_general->getWidget());
}

void SettingsTabGame::setupGeneral(std::shared_ptr<SettingsGroup>) { }

void SettingsTabGame::setupVoicePacksGroup() {
    _voicepacks = std::make_shared<SettingsGroup>("Voice Packs");

    setupVoicePacks(_voicepacks);

    _tabLayout->addWidget(_voicepacks->getWidget());
}

void SettingsTabGame::setupVoicePacks(std::shared_ptr<SettingsGroup> group) {
    _cbEN = std::make_shared<SettingsCheckbox>("English");
    group->addControl(_cbEN);

    _cbJA = std::make_shared<SettingsCheckbox>("日本語");
    group->addControl(_cbJA);

    _cbKR = std::make_shared<SettingsCheckbox>("한글");
    group->addControl(_cbKR);

    _cbZH = std::make_shared<SettingsCheckbox>("简体中文");
    group->addControl(_cbZH);
}

void SettingsTabGame::setupMiscGroup() {
    _misc = std::make_shared<SettingsGroup>("Miscellaneous");

    setupGeneral(_misc);

    _tabLayout->addWidget(_misc->getWidget());
}

void SettingsTabGame::setupMisc(std::shared_ptr<SettingsGroup>) { }

void SettingsTabGame::parse(std::shared_ptr<Settings> data) {
    _cbEN->setChecked(data->lang.lang_game_en);
    _cbJA->setChecked(data->lang.lang_game_jp);
    _cbKR->setChecked(data->lang.lang_game_kr);
    _cbZH->setChecked(data->lang.lang_game_zh);
}