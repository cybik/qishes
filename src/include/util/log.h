//
// Created by cybik on 24-07-13.
//

#pragma once

#include <memory>

#include <QString>

class Log {
public:
    static std::shared_ptr<Log> get_logger();
    void warning(const QString& message);
    void debug(const QString& message);
    void critical(const QString& message);
    void info(const QString& message);
private:
    Log() = default;
    static std::shared_ptr<Log> log_instance;
};

