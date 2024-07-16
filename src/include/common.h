//
// Created by cybik on 24-06-27.
//

#pragma once

#include <QString>

#include <util/log.h>

static const QString APP_VERSION = "0.0.1";

#define APPLICATION_BASE_NAME "moe.cybik.wishes"
#define APPLICATION_NAME_GENERATOR(X) APPLICATION_BASE_NAME#X
#define SPEC_TRANSLATE(X) QCoreApplication::translate(CommandSpecifier.toStdString().c_str(), ""#X )
