//
// Created by cybik on 23-02-17.
//

#ifndef QAGL_RUNNERCOMMON_H
#define QAGL_RUNNERCOMMON_H

#include <string>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <functional>
#include <memory>

#include <data/remote/RemoteCommon.h>

class RunnerRelease {
    RunnerRelease() = delete;

    QString _file;
    QString _release;
    QString _name;
public:
    QString& getName();
    QString& getRelease();
    QString& getFile();
    RunnerRelease(QJsonObject);
};

class RunnerCommon : protected RemoteCommon {
protected:
    //void setup(QUrl, std::function<void(QNetworkReply *)>);
    virtual void handle(QNetworkReply*) = 0; // pure virtual
    void parse_in(const QByteArray&);
    std::shared_ptr<QJsonDocument> _doc;

    QString getRunnerURL();
    QString to_string();

    std::vector<RunnerRelease>& getReleases();
private:
    QString _key;
    QJsonObject _metadata;
    QString _name;
    QString _url_upstream;

    QJsonArray _releases;
    std::vector<RunnerRelease> _rels;
public:
    QString getRootKey();
    QString getRunnerName();
};


#endif //QAGL_RUNNERCOMMON_H
