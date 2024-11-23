//
// Created by cybik on 22-07-24.
//

#include "SettingsTab.h"

SettingsTab::SettingsTab() {
    _setupRibbonPage();
}

bool SettingsTab::indexMatches(int against) {
    return _index == against;
}

int SettingsTab::getIndex() {
    return _index;
}

void SettingsTab::addTab(Nedrysoft::Ribbon::RibbonWidget* ri, const QString& name) {
    _tabLayout->addStretch();
    _index = ri->addTab(_tabWidget.get(), name);
}

void SettingsTab::_setupRibbonPage() {
    _tabWidget = std::make_shared<QWidget>();
    _tabLayout = std::make_shared<QHBoxLayout>(_tabWidget.get());
    _tabLayout->setSpacing(0);
    _tabLayout->setContentsMargins(0,0,0,0);
}

void SettingsTab::trigger(Nedrysoft::Ribbon::RibbonWidget* ri) {
    ri->setCurrentIndex(_index);
}