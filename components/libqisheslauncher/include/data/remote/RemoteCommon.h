//
// Created by cybik on 23-02-26.
//

#ifndef QAGL_REMOTECOMMON_H
#define QAGL_REMOTECOMMON_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

//#include <data/remote/RemoteCommon.h>

class RemoteCommon {
protected:
    void setup(QUrl, std::function<void(QNetworkReply *)>);
    virtual void handle(QNetworkReply*) = 0; // pure virtual
};


#endif //QAGL_REMOTECOMMON_H
