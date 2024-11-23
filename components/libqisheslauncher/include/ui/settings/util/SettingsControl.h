//
// Created by cybik on 22-07-24.
//

#ifndef QAGL_SETTINGSCONTROL_H
#define QAGL_SETTINGSCONTROL_H

#include <QLayout>
#include <QFormLayout>
#include <string>
#include "3rdParty/nedrysoft/Ribbon/include/RibbonGroup"
#include <memory>

class SettingsControl : public QObject {
Q_OBJECT
public:
    explicit SettingsControl(const QString&);
    virtual QLayout* getLayout() = 0;

    void finalize();
protected:
    virtual void addToLayout() = 0;
    virtual void setupEventHandlers() = 0;
    std::shared_ptr<QGridLayout> _layout;
    std::shared_ptr<QFormLayout> _formLayout;

    QString _name;
    void onAnyChange();
};

class SettingsDataControl : public SettingsControl {
public:
    explicit SettingsDataControl(const QString&);
protected:
    bool _first_run = true;
};

#endif //QAGL_SETTINGSCONTROL_H
