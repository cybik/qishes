//
// Created by cybik on 23-02-03.
//

#ifndef QAGL_LAUNCH_H
#define QAGL_LAUNCH_H
#include <memory>
#include <QString>
#include <QStringList>

#include "process.h"
class Launch {
public:
    static std::shared_ptr<Launch> getLaunch();

    void LaunchIt(qint64* pid);
private:
    static std::shared_ptr<Launch> _launch;
    Launch();

    std::unique_ptr<Process> _process;
};

#endif //QAGL_LAUNCH_H
