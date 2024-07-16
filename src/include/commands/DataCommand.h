//
// Created by cybik on 24-07-10.
//

#pragma once

#include "AbstractCommand.h"
#include <QString>
#include <QDir>
#include <data/wishlog.h>

class DataCommand : public AbstractCommand {
public:
    static const QString CommandSpecifier;
    DataCommand() = default;
    void cmd_main(int, char **) override;

protected:
private:
    std::string get_local_storage_folder(WishLog::WishLogGame game);
    QString command_known_url;

    void initialize(WishLog& log);

    QDir data_dir;

};
