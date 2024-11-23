//
// Created by cybik on 22-07-24.
//

#ifndef QAGL_SETTINGSGROUP_H
#define QAGL_SETTINGSGROUP_H

#include "3rdParty/nedrysoft/Ribbon/include/RibbonGroup"
#include <string>
#include <QVBoxLayout>
#include <QSpacerItem>

#include "SettingsControls.h"

class SettingsGroup {
public:
    explicit SettingsGroup(const QString&);
    QWidget* getWidget();
    void addControl(std::shared_ptr<SettingsControl>);
private:
    std::shared_ptr<Nedrysoft::Ribbon::RibbonGroup> _group;
    std::shared_ptr<QVBoxLayout> _groupLayout;

    bool stretched = false;
    int _control_count = 0;
};


#endif //QAGL_SETTINGSGROUP_H
