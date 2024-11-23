//
// Created by cybik on 22-07-24.
//

#ifndef QAGL_SETTINGSWINDOW_H
#define QAGL_SETTINGSWINDOW_H

#include "3rdParty/nedrysoft/Ribbon/include/RibbonWidget"
#include "3rdParty/nedrysoft/Ribbon/include/RibbonGroup"
#include <QMainWindow>
#include <memory>
#include <functional>

#include "SettingsTabs.h"
#include <QtWebEngineWidgets/QtWebEngineWidgets>

#include "launcher/data/SettingsData.h"

class SettingsWindow {
public:
    SettingsWindow(bool usedAsWidget = false);
    void showing();
    void show();
    QWidget* getWidget();

    void setConfig(std::shared_ptr<Settings> ptr);
    void setExitCallback(std::function<void()> onExitSettings);
private:
    std::function<void()> _onExitSettings;
    void onTabBarClicked(int);
    Nedrysoft::Ribbon::RibbonWidget* setup(bool usedAsWidget);
    std::shared_ptr<QWebEngineView> setupGol(bool usedAsWidget);
    std::shared_ptr<QMainWindow> _settingsWindow;

    std::shared_ptr<QVBoxLayout> _settingsLayout;
    QWidget* _settingsWidget;

    //Nedrysoft::Ribbon::RibbonWidget* ri;
    Nedrysoft::Ribbon::RibbonWidget* ri = nullptr;

    // meta-principle: a group is a widget hbox containing a bunch of ribbongroups?
    std::shared_ptr<SettingsTabButtonBack> _tabButtonBack;
    std::shared_ptr<SettingsTabGeneral> _tabGeneral;
    std::shared_ptr<SettingsTabGame> _tabGame;
    std::shared_ptr<SettingsTabEnhancements> _tabEnhancements;
    std::shared_ptr<SettingsTabRunner> _tabRunner;
    std::shared_ptr<SettingsTabMore> _tabMore;
    std::shared_ptr<QWebEngineView> settings_WebEngine;

    std::shared_ptr<Settings> _config;
};


#endif //QAGL_SETTINGSWINDOW_H
