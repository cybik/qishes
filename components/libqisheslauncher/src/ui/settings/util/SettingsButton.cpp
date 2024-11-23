//
// Created by cybik on 22-07-24.
//

#include "SettingsButton.h"

SettingsButton::SettingsButton(const QString& name) : SettingsControl(name){
    _control = std::make_shared<Nedrysoft::Ribbon::RibbonButton>();
    _control->setText(name);
    finalize();
}

void SettingsButton::addToLayout() {
    _formLayout->addRow(_control.get());
}

QLayout* SettingsButton::getLayout() {
    return _layout->layout();
}

void SettingsButton::setupEventHandlers() {
    connect(
        _control.get(), &Nedrysoft::Ribbon::RibbonButton::clicked,
        [&](bool checked) {onClick(checked);}
    );
}

void SettingsButton::onClick(bool isChecked) {
    if(_handler) {
        (*_handler)(isChecked);
    }
}

void SettingsButton::addHandler(std::unique_ptr<std::function<void(bool)>> _in) {
    _handler = std::move(_in);
}
