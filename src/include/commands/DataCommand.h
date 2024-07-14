//
// Created by cybik on 24-07-10.
//

#pragma once

#include "AbstractCommand.h"
#include <QString>
#include <data/wishlog.h>

class DataCommand : public AbstractCommand {
public:
    static const QString CommandSpecifier;
    DataCommand() = default;
    void cmd_main(int, char **) override;

protected:
private:
    QString command_known_url;

};
