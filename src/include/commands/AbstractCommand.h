/******************************************************************
 *
 * AbstractCommand.h
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-06-27 for qgachawishes.
 *
 ******************************************************************/
#pragma once

#include <QString>
#include <QFile>

#include <common.h>
#include <QFileSystemModel>
#include <QCommandLineParser>
#include <data/wishlog.h>
#include <QApplication>

#include <discord.h>

class AbstractCommand {

public:
    int cmd_main(int, char **);
protected:

    virtual void command_create_application(int& argc, char **argv) = 0;
    virtual void command_setup_parser() = 0;
    virtual void command_process_parser() = 0;
    virtual int  command_run() = 0;

    QString command_game_path;
    QString command_file_path;

    // common calls
    std::shared_ptr<std::list<std::shared_ptr<QFile>>> getGameWishesCache();
    void printSingleFilePath(const QString& filename);
    std::shared_ptr<QStringList> runUrlSearch(const std::shared_ptr<QFile>& qfile);
    std::shared_ptr<QStringList> runUrlCleanup(const std::shared_ptr<QStringList>& ptr);
    std::shared_ptr<std::list<WishLog>> runFilterForLogs(const std::shared_ptr<QStringList>& ptr);

    void warnHelp(int exit_code, const QString& message = "");
    std::shared_ptr<QCommandLineParser> parser;
private:
};
