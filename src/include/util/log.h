/******************************************************************
 * log.h
 *
 * Created by cybik on 24-07-13. for qgachawishes.
 *
 ******************************************************************/
#pragma once

#include <memory>
#include <QString>

class Log {
public:
    typedef enum {
        Debug,
        Info,
        Warning,
        Critical
    } LogLevel;
    static std::shared_ptr<Log> get_logger();
    void warning(const QString& message);
    void debug(const QString& message);
    void critical(const QString& message);
    void info(const QString& message);
    void log_level(LogLevel level);
private:
    void print(const QString& message);
    Log() = default;
    static std::shared_ptr<Log> mlogInstance;

    LogLevel mlogLevel = LogLevel::Debug;
};

