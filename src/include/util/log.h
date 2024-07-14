//
// Created by cybik on 24-07-13.
//

#pragma once

#include <memory>

#include <QString>

class Log {
public:
    static std::shared_ptr<Log> get_logger();
    void warning(QString message);
    void debug(QString message);
    void critical(QString message);
    void info(QString message);
private:
    Log() = default;
    static std::shared_ptr<Log> log_instance;
};

