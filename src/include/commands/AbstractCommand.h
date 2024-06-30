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
    virtual void process(int, char **) = 0;
protected:
    QString command_game_path;
    std::unique_ptr<std::list<std::unique_ptr<QFile>>> getGameWishesCache();
private:
};
