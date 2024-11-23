//
// Created by cybik on 22-07-24.
//

#ifndef QAGL_SETTINGSLABEL_H
#define QAGL_SETTINGSLABEL_H

#include "3rdParty/nedrysoft/Ribbon/include/RibbonCheckBox"
#include <string>
#include <QHBoxLayout>
#include <QLabel>
#include <memory>

#include "SettingsControl.h"

class SettingsLabel : public SettingsControl {
Q_OBJECT
public:
    explicit SettingsLabel(const QString& name);
    QLayout* getLayout() override;
    std::shared_ptr<QLabel> getControl() { return _control; }
    void setText(const QString& name);
    void setColor(const QColor& color);
protected:
    void addToLayout() override;
    void setupEventHandlers() override;
private:
    std::shared_ptr<QLabel> _control;
};


#endif //QAGL_SETTINGSCOMBO_H
