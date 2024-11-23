//
// Created by cybik on 23-02-03.
//

#include <process/launch.h>

const QString _genpath = "/Depot/Games/Genshin/Game/drive_c/Program Files/Genshin Impact";
const QString _winepath = "/home/cybik/.steam/root/compatibilitytools.d/proton-latest/files/bin/wine";
const QString _executable = "GenshinImpact.exe";
const QString _winepfx = "/Depot/Games/Genshin/GameTrash2";

std::shared_ptr<Launch> Launch::_launch = nullptr;

std::shared_ptr<Launch> Launch::getLaunch() {
    if(_launch == nullptr) {
        _launch.reset(new Launch());
    }
    return _launch;
}

Launch::Launch() {
    auto qpe = QProcessEnvironment();
    qpe.insert("WINEPREFIX", _winepfx);

    _process = std::make_unique<Process>(_winepath, _executable, _genpath, QStringList(), qpe);
}

void Launch::LaunchIt(qint64* pid) {
    _process->start(pid);
}
