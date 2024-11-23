//
// Created by cybik on 22-07-24.
//

#ifndef QAGL_SETTINGSCOMBO_H
#define QAGL_SETTINGSCOMBO_H

#include "3rdParty/nedrysoft/Ribbon/include/RibbonComboBox"
#include <string>
#include <QHBoxLayout>
#include <QLabel>
#include <memory>

#include "SettingsControl.h"

class SettingsCombo : public SettingsDataControl {
Q_OBJECT
public:
    explicit SettingsCombo(const QString& name);
    QLayout* getLayout() override;
    void addChoice(const char* choice);
    void addChoice(const QString&, const QString&);
    void addChoices(int count, ...);
protected:
    void addToLayout() override;
    void setupEventHandlers() override;
private:
    std::shared_ptr<Nedrysoft::Ribbon::RibbonComboBox> _control;
public slots:
    void onIndexChanged(int);
};


#endif //QAGL_SETTINGSCOMBO_H
