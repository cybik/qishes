/******************************************************************
**
** HistoryCommand.h
**
** \brief A brief summary of what this class intends to accomplish.
**
** Created by cybik on 24-06-27 for qgachawishes.
**
*******************************************************************/
#pragma once

#include "AbstractCommand.h"
#include <QString>

class HistoryCommand : public AbstractCommand {
public:
    static const QString CommandSpecifier;
    HistoryCommand() = default;

protected:
    void command_create_application(int& argc, char **argv) override;
    void command_setup_parser() override;
    void command_process_parser() override;
    int  command_run() override;

private:
    bool command_reverse_order;
    bool command_open_url;
    int command_max_return_num;
    std::shared_ptr<QCoreApplication> qwishes_history = nullptr;

    // arguments
    std::shared_ptr<QCommandLineOption> game_path, file_path, reverse_order, open_url, max_return_num;

};
