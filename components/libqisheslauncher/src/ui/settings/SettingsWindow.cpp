//
// Created by cybik on 22-07-24.
//

#include "SettingsWindow.h"

#include <QEvent>
#include <iostream>

SettingsWindow::SettingsWindow(bool usedAsWidget) {
    _settingsWindow = std::make_shared<QMainWindow>();
    _settingsWindow->setWindowTitle("QAGL Settings");

    _settingsWindow->setFixedSize(800,580);
    _settingsWindow->setWindowModality(Qt::ApplicationModal);

    _settingsLayout = std::make_shared<QVBoxLayout>();
    _settingsLayout->setContentsMargins(0,0,0,0);
    _settingsLayout->setSpacing(0);
    _settingsLayout->addWidget(setup(usedAsWidget));
    _settingsLayout->addWidget(setupGol(usedAsWidget).get());
    _settingsWidget = new QWidget();
    _settingsWidget->setLayout(_settingsLayout.get());
    _settingsWindow->setCentralWidget(_settingsWidget);
}

void SettingsWindow::setExitCallback(std::function<void()> onExitSettings) {
    _onExitSettings = onExitSettings;
}

void SettingsWindow::onTabBarClicked(int tab_index) {
    if(_tabButtonBack && _tabButtonBack->indexMatches(tab_index)) {
        _onExitSettings();
    }
}

Nedrysoft::Ribbon::RibbonWidget* SettingsWindow::setup(bool usedAsWidget) {
    if(ri == nullptr) {
        ri = new Nedrysoft::Ribbon::RibbonWidget();
        if(usedAsWidget) {
            // add back arrow.
            _tabButtonBack = std::make_shared<SettingsTabButtonBack>(ri);
            //ri->tabBar()->installEventFilter(this);
            QObject::connect(
                ri->tabBar(), &QTabBar::tabBarClicked,
                [&](int tab_index) {
                    onTabBarClicked(tab_index);
                }
            );
        }
        _tabGeneral = std::make_shared<SettingsTabGeneral>(ri);
        _tabGame = std::make_shared<SettingsTabGame>(ri);
        _tabRunner = std::make_shared<SettingsTabRunner>(ri);
        _tabEnhancements = std::make_shared<SettingsTabEnhancements>(ri);
        _tabMore = std::make_shared<SettingsTabMore>(ri);
        ri->setFixedHeight(ri->height() + 30);
    }
    ri->tabBar()->setCurrentIndex(_tabGeneral->getIndex());

    return ri;
}

std::shared_ptr<QWebEngineView> SettingsWindow::setupGol(bool usedAsWidget) {
    if(settings_WebEngine == nullptr) {
        settings_WebEngine = std::make_shared<QWebEngineView>();
        settings_WebEngine->setContextMenuPolicy(Qt::NoContextMenu);
        settings_WebEngine->setAcceptDrops(false);
        settings_WebEngine->load(QUrl("https://launcher.moe"));
    }
    return settings_WebEngine;
}

void SettingsWindow::showing() {
    ri->tabBar()->setCurrentIndex(_tabGeneral->getIndex());
}

void SettingsWindow::show() {
    _settingsWindow->show();
}

QWidget* SettingsWindow::getWidget() {
    return _settingsWidget;
}

void SettingsWindow::setConfig(std::shared_ptr<Settings> ptr) {
    if(_config != ptr) {
        _config = ptr;
        if(_tabGeneral)         _tabGeneral->parse(ptr);
        if(_tabGame)            _tabGame->parse(ptr);
        if(_tabRunner)          _tabRunner->parse(ptr);
        if(_tabEnhancements)    _tabEnhancements->parse(ptr);
        if(_tabMore)            _tabMore->parse(ptr);
        if(ptr) {
            //std::cout << _config->to_string() << std::endl;
        }
    }
}
