//
// Created by cybik on 22-07-24.
//

#ifndef QAGL_SETTINGSTAB_H
#define QAGL_SETTINGSTAB_H

#include <QWidget>
#include <QHBoxLayout>
#include <memory>
#include "3rdParty/nedrysoft/Ribbon/include/RibbonWidget"

#include "launcher/data/Settings.h"
#include "launcher/data/SettingsData.h"

class SettingsTab {
public:
    SettingsTab();
    void trigger(Nedrysoft::Ribbon::RibbonWidget*);
    bool indexMatches(int against);
    int getIndex();
    virtual void parse(std::shared_ptr<Settings>) = 0;
protected:
    void addTab(Nedrysoft::Ribbon::RibbonWidget*, const QString&);
    std::shared_ptr<QWidget> _tabWidget;
    std::shared_ptr<QHBoxLayout> _tabLayout;
private:
    void _setupRibbonPage();

    int _index = -1;
};


#endif //QAGL_SETTINGSTAB_H
