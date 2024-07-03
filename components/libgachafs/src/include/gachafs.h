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
    static std::shared_ptr<std::list<std::shared_ptr<QFile>>> getFiles(const QString& filter, QString& game_path);
protected:
private:
    static int seek_depth(int base_level, const QStringList& levels_check, const QFileInfo& compare);
    gachafs() = default;
    ~gachafs() = default;
};
