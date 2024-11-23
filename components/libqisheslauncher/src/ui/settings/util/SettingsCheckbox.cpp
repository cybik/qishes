//
// Created by cybik on 22-07-24.
//

#include "SettingsCheckbox.h"

#include <data/SettingsData.h>

SettingsCheckbox::SettingsCheckbox(const QString& name) : SettingsDataControl(name){
    _control = std::make_shared<Nedrysoft::Ribbon::RibbonCheckBox>();
    _control->setText(name);
    finalize();
}

void SettingsCheckbox::addToLayout() {
    _formLayout->addRow(_control.get());
}

QLayout* SettingsCheckbox::getLayout() {
    return _layout->layout();
}

void SettingsCheckbox::setupEventHandlers() {
    connect(
        _control.get(), &Nedrysoft::Ribbon::RibbonCheckBox::stateChanged,
        [this](int _state) {
            if(this->_first_run) {
                this->_first_run = false;
                return;
            }
            onStateChanged(_state);
            onAnyChange();
        }
    );
}

void SettingsCheckbox::setChecked(bool checked) {
    _control->setChecked(checked);
}

bool SettingsCheckbox::isChecked() {
    return _control->isChecked();
}

void SettingsCheckbox::onStateChanged(int newState) {
    // save
    SettingsData::getSettingsData()->update();
}