//
// Created by cybik on 24-06-27.
//

#pragma once

#include "AbstractCommand.h"
#include <QString>

class HistoryCommand : public AbstractCommand {
public:
    static const QString CommandSpecifier;
    HistoryCommand();
    void process(int, char **) override;
protected:
private:
    std::shared_ptr<QStringList> runUrlSearch(std::shared_ptr<QFile> qfile);
    std::shared_ptr<QStringList> runUrlCleanup(std::shared_ptr<QStringList> ptr);
    std::shared_ptr<QStringList> runFilterForLogs(std::shared_ptr<QStringList> ptr);
    bool command_reverse_order;
    bool command_open_url;
    int command_max_return_num;

};
