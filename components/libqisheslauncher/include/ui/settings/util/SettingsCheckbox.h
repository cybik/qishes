//
// Created by cybik on 22-07-24.
//

#ifndef QAGL_SETTINGSCHECK_H
#define QAGL_SETTINGSCHECK_H

#include "3rdParty/nedrysoft/Ribbon/include/RibbonCheckBox"
#include <string>
#include <QHBoxLayout>
#include <QLabel>
#include <memory>

#include "SettingsControl.h"

class SettingsCheckbox : public SettingsDataControl {
Q_OBJECT
public:
    void setChecked(bool);
    bool isChecked();
    explicit SettingsCheckbox(const QString& name);
    QLayout* getLayout() override;
protected:
    void addToLayout() override;
    void setupEventHandlers() override;
private:
    std::shared_ptr<Nedrysoft::Ribbon::RibbonCheckBox> _control;
    void onStateChanged(int);

};


#endif //QAGL_SETTINGSCOMBO_H
