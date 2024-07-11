//
// Created by cybik on 24-06-27.
//

#pragma once

#include <QString>
#include <QFile>

#include <common.h>
#include <QFileSystemModel>

class AbstractCommand {

public:
    virtual void cmd_main(int, char **) = 0;
protected:
    QString command_game_path;
    std::shared_ptr<std::list<std::shared_ptr<QFile>>> getGameWishesCache();
private:
};
