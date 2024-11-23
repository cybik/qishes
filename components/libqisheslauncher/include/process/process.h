//
// Created by cybik on 23-02-03.
//

#ifndef QAGL_PROCESS_H
#define QAGL_PROCESS_H

#include <algorithm>
#include <QProcess>

#include <functional>
#include <memory>

class Process {
public:
    Process(const QString p, const QString e, const QString d, const QStringList a, const QProcessEnvironment q);
    qint64 start(qint64*);
private:
    std::unique_ptr<QProcess> _proc;
    QString _command;
    QStringList _arguments;

    qint64 _pid = -1;
};

#endif //QAGL_PROCESS_H
