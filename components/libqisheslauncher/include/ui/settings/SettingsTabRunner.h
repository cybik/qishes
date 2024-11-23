//
// Created by cybik on 22-07-24.
//

#ifndef QAGL_SETTINGSTABRUNNER_H
#define QAGL_SETTINGSTABRUNNER_H

#include "SettingsTab.h"
#include "3rdParty/nedrysoft/Ribbon/include/RibbonWidget"
#include "3rdParty/nedrysoft/Ribbon/include/RibbonGroup"
#include <QWidget>
#include <QHBoxLayout>

#include "launcher/ui/settings/util/SettingsControls.h"
#include "launcher/ui/settings/util/SettingsGroup.h"

#include "launcher/data/remote/runners/Proton.h"
#include "launcher/data/remote/runners/Wine.h"

#include <tuple>

class SettingsTabRunner : public SettingsTab {
public:
    explicit SettingsTabRunner(Nedrysoft::Ribbon::RibbonWidget* ri);
    void parse(std::shared_ptr<Settings>) override;
private:
    std::shared_ptr<SettingsGroup> _runWINE;
    std::shared_ptr<SettingsCombo> _cbWINE;
    std::shared_ptr<SettingsGroup> _runDXVK;

    void setupRunnerGroup();
    void setupDXVKGroup();

    void setupRunners(std::shared_ptr<SettingsGroup> group);
    void setupDXVKs(std::shared_ptr<SettingsGroup> group);

    std::vector<std::tuple<std::string, std::shared_ptr<RunnerCommon>>> _runners;
};



#endif //QAGL_SETTINGSTABRUNNER_H
