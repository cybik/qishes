//
// Created by cybik on 22-07-24.
//

#include "SettingsTabButtonBack.h"

SettingsTabButtonBack::SettingsTabButtonBack(Nedrysoft::Ribbon::RibbonWidget* ri) : SettingsTab() {
    addTab(ri, "< Back");
}
