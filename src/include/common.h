//
// Created by cybik on 24-06-27.
//

#pragma once

#include <QString>

#include <util/log.h>

static const QString APP_VERSION = "0.0.1";

#define APPNAME_BASE "moe.cybik.wishes"
#define APPNAME_GEN(X) APPNAME_BASE#X
#define L18N_M(X) QApplication::translate("main", ""#X )
#define L18N(X) QApplication::translate(CommandSpecifier.toStdString().c_str(), ""#X )
