//
// Created by cybik on 24-06-27.
//

#pragma once

#include "AbstractCommand.h"
#include <QString>
#include <data/wishlog.h>

class HistoryCommand : public AbstractCommand {
public:
    static const QString CommandSpecifier;
    HistoryCommand() = default;
    void cmd_main(int, char **) override;
protected:
private:
    std::shared_ptr<QStringList> runUrlSearch(const std::shared_ptr<QFile>& qfile);
    std::shared_ptr<QStringList> runUrlCleanup(const std::shared_ptr<QStringList>& ptr);
    std::shared_ptr<std::list<WishLog>> runFilterForLogs(const std::shared_ptr<QStringList>& ptr);
    bool command_reverse_order;
    bool command_open_url;
    int command_max_return_num;

};
