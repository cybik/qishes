//
// Created by cybik on 24-07-13.
//
#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#include <util/log.h>
#include <termcolor/termcolor.hpp>

std::shared_ptr<Log> Log::mlogInstance = nullptr;

std::shared_ptr<Log> Log::get_logger() {
    if(!mlogInstance) {
        mlogInstance = std::shared_ptr<Log>(new Log());
    }
    return mlogInstance;
}

void Log::debug(const QString& message) {
    if(mlogLevel > LogLevel::Debug) return;
    std::cout
        << termcolor::bold
            << termcolor::bright_green
            << "[DEBUG] "
        << termcolor::reset
            << termcolor::green
            << message.toStdString()
        << termcolor::reset
        << std::endl;
}

void Log::info(const QString& message) {
    if(mlogLevel > LogLevel::Info) return;
    std::cout
        << termcolor::bold
            << termcolor::bright_cyan
            << "[INFO] "
        << termcolor::reset
            << termcolor::cyan
            << message.toStdString()
        << termcolor::reset
        << std::endl;
}

void Log::warning(const QString& message) {
    if(mlogLevel > LogLevel::Warning) return;
    std::cout
        << termcolor::bold
            << termcolor::bright_yellow
            << "[WARNING] "
        << termcolor::reset
            << termcolor::yellow
            << message.toStdString()
        << termcolor::reset
        << std::endl;
}

void Log::critical(const QString& message) {
    if(mlogLevel > LogLevel::Critical) return;
    std::cout
        << termcolor::bold
            << termcolor::bright_red
            << "[CRITICAL] "
        << termcolor::reset
            << termcolor::red
            << message.toStdString()
        << termcolor::reset
        << std::endl;
}

void Log::log_level(Log::LogLevel level) {
    mlogLevel = level;
}

#pragma clang diagnostic pop
