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
    int cmd_main(int, char **) override;


protected:
private:
    std::string get_local_storage_folder(WishLog::WishLogGame game);
    void early_exit(const QString& message, int exit_code);
    QString command_known_url;
    bool command_all_targets;

    void run_data_sync(WishLog& log);
    void check_initial_doc(QJsonDocument& doc);

    void start_sync_process(WishLog& log, QNetworkReply* reply);
    void process_initial_data(WishLog& log, QNetworkReply* reply);
    QDir data_dir;

    std::map<QString, QJsonDocument> loaded_data;
    std::shared_ptr<QApplication> qwishes_data;
};
