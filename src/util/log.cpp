/******************************************************************
 *
 * log.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-07-13. for qgachawishes.
 *
 ******************************************************************/
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
    if(mlogLevel > LogLevel::Debug || message.isEmpty()) return;
    std::cout
        << termcolor::bold
            << termcolor::on_bright_green
            << "[DEBUG]"
        << termcolor::reset
            << termcolor::green
            << " "
            << message.toStdString()
        << termcolor::reset
        << std::endl;
}

void Log::info(const QString& message) {
    if(mlogLevel > LogLevel::Info || message.isEmpty() ) return;
    std::cout
        << termcolor::bold
            << termcolor::on_bright_cyan
            << "[INFO]"
        << termcolor::reset
            << termcolor::cyan
            << " "
            << message.toStdString()
        << termcolor::reset
        << std::endl;
}

void Log::warning(const QString& message) {
    if(mlogLevel > LogLevel::Warning || message.isEmpty() ) return;
    std::cout
        << termcolor::bold
            << termcolor::on_bright_yellow
            << "[WARNING]"
        << termcolor::reset
            << termcolor::yellow
            << " "
            << message.toStdString()
        << termcolor::reset
        << std::endl;
}

void Log::critical(const QString& message) {
    if( mlogLevel > LogLevel::Critical || message.isEmpty() ) return;
    std::cout
        << termcolor::bold
            << termcolor::on_bright_red
            << "[CRITICAL]"
        << termcolor::reset
            << termcolor::red
            << " "
            << message.toStdString()
        << termcolor::reset
        << std::endl;
}

void Log::log_level(Log::LogLevel level) {
    mlogLevel = level;
}

#pragma clang diagnostic pop
