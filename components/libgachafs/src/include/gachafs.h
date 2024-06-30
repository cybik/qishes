//
// Created by cybik on 24-06-29.
//

#pragma once

#include <iostream>

#include <QString>
#include <QFile>

#include <QFileSystemModel>

class gachafs {
public:
    static std::unique_ptr<std::list<std::unique_ptr<QFile>>> getFiles(const QString& filter, QString& command_game_path);
protected:
private:
    gachafs();
    ~gachafs();
};
