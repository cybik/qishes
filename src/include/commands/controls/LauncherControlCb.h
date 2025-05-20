/******************************************************************
 *
 * LauncherControlCb.h
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-05-20, for qgachawishes.
 *
 ******************************************************************/
#pragma once

#include <SARibbonBar/SARibbonCheckBox.h>

class LauncherControlCb {
public:
    static std::unique_ptr<LauncherControlCb> make_me(
        QString, QString, QString, bool, QString, QString, std::function<void(Qt::CheckState)> = nullptr
    );
    LauncherControlCb(QString, QString, QString, bool, QString, QString, std::function<void(Qt::CheckState)> = nullptr);
    SARibbonCheckBox* getCbControl();
    std::string getValue();
    std::string getEnvName();
    bool isChecked();
    ~LauncherControlCb();
protected:
private:
    std::unique_ptr<SARibbonCheckBox> _cb = nullptr;
    QString ret_true;
    QString ret_false;
    QString env_name;
};
