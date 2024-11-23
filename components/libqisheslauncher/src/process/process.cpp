//
// Created by cybik on 23-02-03.
//

#include <process/process.h>

#include <QProcessEnvironment>

Process::Process(const QString program, const QString executable, const QString cwd,
                 const QStringList args, const QProcessEnvironment qpe
) {
    _proc = std::make_unique<QProcess>();

    _proc->setProgram(program);

    _proc->setWorkingDirectory(cwd);

    auto _args = args;
    _args.insert(0, executable);
    _proc->setArguments(args);

    auto _env = QProcessEnvironment::systemEnvironment();
    _env.insert(qpe);
    _proc->setProcessEnvironment(_env);
}

qint64 Process::start(qint64* pid) {
    _proc->startDetached(pid);
    _proc->waitForStarted();
    return *pid;
}
