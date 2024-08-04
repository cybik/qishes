//
// Created by cybik on 24-06-27.
//

#pragma once

#include "AbstractCommand.h"
#include <QString>

class HistoryCommand : public AbstractCommand {
public:
    static const QString CommandSpecifier;
    HistoryCommand() = default;
    int cmd_main(int, char **) override;
protected:
private:
    bool command_reverse_order;
    bool command_open_url;
    int command_max_return_num;
    std::shared_ptr<QApplication> qwishes_history;

};
