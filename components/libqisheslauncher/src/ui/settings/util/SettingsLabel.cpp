//
// Created by cybik on 22-07-24.
//

#include "SettingsLabel.h"

SettingsLabel::SettingsLabel(const QString& name) : SettingsControl(name){
    _control = std::make_shared<QLabel>();
    finalize();
    setText(name);
}

void SettingsLabel::setText(const QString &text) {
    _control->setText(text);
}
void SettingsLabel::setColor(const QColor &color) {
    QPalette pal = _control->palette();
    pal.setColor(_control->foregroundRole(), color);
    _control->setPalette(pal);
}

void SettingsLabel::addToLayout() {
    _formLayout->addRow(_control.get());
}


QLayout* SettingsLabel::getLayout() {
    return _layout->layout();
}

void SettingsLabel::setupEventHandlers() {
    // noop
}