//
// Created by cybik on 24-07-10.
//

#pragma once

#include "AbstractCommand.h"
#include <QString>
#include <QDir>
#include <qnetworkreply.h>
#include <data/wishlog.h>

#include <QJsonDocument>

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

    void process_with_initial_data(WishLog& log, QNetworkReply* reply);
    void load_local_data();
    QDir data_dir;

    std::map<QString, QJsonDocument> loaded_data;
    QCoreApplication* qwishes_data;
};
