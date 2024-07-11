//
// Created by cybik on 24-07-10.
//

#include <commands/DaemonCommand.h>
const QString DaemonCommand::CommandSpecifier = "daemon";

void DaemonCommand::cmd_main(int, char **) {
    abort();
}