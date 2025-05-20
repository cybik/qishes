/******************************************************************
 *
 * LauncherControlCb.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-05-20, for qgachawishes.
 *
 ******************************************************************/

#include <commands/controls/LauncherControlCb.h>

std::unique_ptr<LauncherControlCb> LauncherControlCb::make_me(
    QString title, QString objname, QString name, bool defaultVal, QString val_true, QString val_false,
    std::function<void(Qt::CheckState)> updateFn
) {
    return std::move(
        std::make_unique<LauncherControlCb>(
            title, objname, name, defaultVal, val_true, val_false, updateFn
        )
    );
}

std::string LauncherControlCb::getValue() {
    return (_cb->isChecked()?ret_true:ret_false).toStdString();
}

bool LauncherControlCb::isChecked() {
    return _cb->isChecked();
}

SARibbonCheckBox* LauncherControlCb::getCbControl() {
    return _cb.get();
}

LauncherControlCb::~LauncherControlCb() {
    _cb.reset();
}

std::string LauncherControlCb::getEnvName() {
    return env_name.toStdString();
}

LauncherControlCb::LauncherControlCb(
    QString title, QString objname, QString name, bool defaultVal, QString val_true, QString val_false,
    std::function<void(Qt::CheckState)> updateFn
) {
    _cb = std::make_unique<SARibbonCheckBox>();
    _cb->setText(title);
    _cb->setObjectName(objname);
    _cb->setChecked(defaultVal);
    env_name = name;
    ret_true = val_true;
    ret_false = val_false;
    if (updateFn) {
        QObject::connect(
            _cb.get(), &QCheckBox::checkStateChanged, updateFn
        );
    }
}
