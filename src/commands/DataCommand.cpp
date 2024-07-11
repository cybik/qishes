//
// Created by cybik on 24-07-10.
//

#include <commands/DataCommand.h>
const QString DataCommand::CommandSpecifier = "data";

void DataCommand::cmd_main(int, char **) {
    abort();
}