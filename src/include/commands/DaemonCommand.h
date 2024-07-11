//
// Created by cybik on 24-07-10.
//

#pragma once

#include "AbstractCommand.h"

class DaemonCommand : public AbstractCommand {
public:
    static const QString CommandSpecifier;
    DaemonCommand() = default;
    void cmd_main(int, char **) override;

protected:
private:

};


