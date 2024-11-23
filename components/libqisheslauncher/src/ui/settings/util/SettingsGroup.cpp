//
// Created by cybik on 22-07-24.
//

#include "SettingsGroup.h"

#define MAX_WIDTH 140

SettingsGroup::SettingsGroup(const QString& name) {
    _group = std::make_shared<Nedrysoft::Ribbon::RibbonGroup>();
    _group->setGroupName(name);
    _group->setMinimumSize(MAX_WIDTH, 0);
    _group->setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
    _groupLayout = std::make_shared<QVBoxLayout>();
    _group->setLayout(_groupLayout.get());
}

void SettingsGroup::addControl(std::shared_ptr<SettingsControl> cb) {
    _groupLayout->addLayout(cb->getLayout());
    _control_count++;
}

QWidget* SettingsGroup::getWidget() {
    if( _control_count < 4 && !stretched) {
        _groupLayout->addStretch();
        stretched = true;
    }
    return _group.get();
}