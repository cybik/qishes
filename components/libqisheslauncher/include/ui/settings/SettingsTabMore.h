//
// Created by cybik on 22-07-24.
//

#ifndef QAGL_SETTINGSTABMORE_H
#define QAGL_SETTINGSTABMORE_H

#include "SettingsTab.h"
#include "3rdParty/nedrysoft/Ribbon/include/RibbonWidget"
#include "3rdParty/nedrysoft/Ribbon/include/RibbonGroup"
#include <QWidget>
#include <QHBoxLayout>


class SettingsTabMore : public SettingsTab {
public:
    explicit SettingsTabMore(Nedrysoft::Ribbon::RibbonWidget* ri);
    void parse(std::shared_ptr<Settings>) override;
private:
    std::shared_ptr<Nedrysoft::Ribbon::RibbonGroup> _moreShaders;
    std::shared_ptr<Nedrysoft::Ribbon::RibbonGroup> _moreEnv;
};


#endif //QAGL_SETTINGSTABMORE_H
