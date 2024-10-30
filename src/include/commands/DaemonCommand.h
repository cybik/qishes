//
// Created by cybik on 24-07-10.
//

#pragma once

#include "AbstractCommand.h"

class DaemonCommand : public AbstractCommand {
public:
    static const QString CommandSpecifier;
    DaemonCommand() = default;

protected:
    void command_create_application(int& argc, char **argv) override;
    void command_setup_parser() override;
    void command_process_parser() override;
    int  command_run() override;
private:

};


