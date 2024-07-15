//
// Created by cybik on 24-06-27.
//

#pragma once

#include <QString>
#include <QFile>

#include <common.h>
#include <QFileSystemModel>
#include <QCommandLineParser>
#include <data/wishlog.h>

class AbstractCommand {

public:
    virtual void cmd_main(int, char **) = 0;
protected:
    QString command_game_path;

    // common calls
    std::shared_ptr<std::list<std::shared_ptr<QFile>>> getGameWishesCache(QCommandLineParser* parser);
    void printSingleFilePath(const QString& filename);
    std::shared_ptr<QStringList> runUrlSearch(const std::shared_ptr<QFile>& qfile);
    std::shared_ptr<QStringList> runUrlCleanup(const std::shared_ptr<QStringList>& ptr);
    std::shared_ptr<std::list<WishLog>> runFilterForLogs(const std::shared_ptr<QStringList>& ptr);
private:
};
