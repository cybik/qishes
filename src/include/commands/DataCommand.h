//
// Created by cybik on 24-07-10.
//

#pragma once

#include "AbstractCommand.h"
#include <QString>
#include <QDir>
#include <qnetworkreply.h>
#include <data/wishlog.h>
#include <utility>

#include <QJsonDocument>
#include <httpclient/httpclient.h>

class DataCommand : public QObject, public AbstractCommand{
    Q_OBJECT
public:
    static const QString CommandSpecifier;
    DataCommand() = default;
    int cmd_main(int, char **) override;

public slots:
    void started();

protected:
private:
    std::string get_local_storage_folder(WishLog::WishLogGame game);
    void early_exit(const QString& message, int exit_code);
    QString command_known_url;
    bool command_all_targets;


    void run_data_sync(WishLog& log);
    void start_sync_process(WishLog& log, QByteArray result);
    void run_sync_loop(WishLog& log, const QString& key, int page = 1);
    void check_initial_doc(QJsonDocument& doc);

    void process_initial_data(WishLog& log, QNetworkReply* reply = nullptr, const QString& reply_as_str = "");
    QDir data_dir;

    void decode_initial_url(WishLog& log);

    std::map<QString, QJsonDocument> loaded_data;
    std::shared_ptr<QCoreApplication> qwishes_data;
    std::shared_ptr<QNetworkAccessManager> qwishes_qnam;
    std::map<QString, std::shared_ptr<QNetworkAccessManager>> qwishes_network_requests;
    std::shared_ptr<std::list<std::shared_ptr<QFile>>> caches;

    std::shared_ptr<HttpClient> mHttpClient;
};
