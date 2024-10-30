//
// Created by cybik on 24-06-27.
//
#pragma clang diagnostic push
#pragma ide diagnostic ignored "performance-unnecessary-value-param"

#include <QDesktopServices>
#include <QRegularExpression>
#include <iostream>
#include <commands/AbstractCommand.h>

#include <termcolor/termcolor.hpp>

#include <gachafs.h>

#include "egachafs.h"

const QString filter = "**/webCaches/**/Cache/Cache_Data/data_2";

std::shared_ptr<std::list<std::shared_ptr<QFile>>> AbstractCommand::getGameWishesCache() {
    try {
        return gachafs::getFiles(filter, this->command_game_path);
    } catch (EGachaFS_Exception& e) {
        warnHelp(0);
    }
    return nullptr;
}

int AbstractCommand::cmd_main(int argc, char **argv) {
    this->command_create_application(argc, argv);
    this->command_setup_parser();
    this->command_process_parser();
    return this->command_run();
}

void AbstractCommand::printSingleFilePath(const QString &filename) {
    std::cout
        << termcolor::bold << termcolor::cyan  << "[#] "        << termcolor::reset
        << termcolor::bold << termcolor::green << "Data file"   << termcolor::reset
        << ": "
        << termcolor::yellow << filename.toStdString() << termcolor::reset
        << std::endl;
}

std::shared_ptr<QStringList> AbstractCommand::runUrlSearch(const std::shared_ptr<QFile>& qfile) {
    if(!qfile->exists()) return nullptr;
    qfile->open(QFile::ReadOnly);
    QRegularExpressionMatchIterator qrem =
        QRegularExpression("1/0/https(.*)\0\0\0\0\0\0\0\0").globalMatch(qfile->readAll());
    std::shared_ptr<QStringList> retList = std::make_shared<QStringList>();
    while(qrem.hasNext()) retList->append(qrem.next().captured(0));
    qfile->close();
    return retList;
}

std::shared_ptr<QStringList> AbstractCommand::runUrlCleanup(const std::shared_ptr<QStringList>& ptr) {
    std::shared_ptr<QStringList> retList; // don't initialize unless necessary
    for(const auto& single_string: (*ptr)) {
        for(const auto& split_string_1: single_string.split("1/0/")) { /** cut on 1/0/ **/
            for(const auto& split_string_2: split_string_1.split('\0', Qt::SkipEmptyParts)) {
                if(split_string_2.startsWith("http")) {
                    if(!retList) retList = std::make_shared<QStringList>();
                    // always split after a cache entry, defined by nullchars
                    retList->append(split_string_2); // first of the second level
                }
            }
        }
    }
    ptr->clear(); /** cleanup **/
    return retList;
}

std::shared_ptr<std::list<WishLog>> AbstractCommand::runFilterForLogs(const std::shared_ptr<QStringList>& ptr) {
    if(!ptr) return nullptr;
    std::shared_ptr<std::list<WishLog>> retList = std::make_shared<std::list<WishLog>>();
    for(const auto& single_string: (*ptr)) {
        if(WishLog::is_accepted_url(single_string)) {
            retList->emplace_front(single_string, WishLog::History); // last is always first. :D
        }
    }
    ptr->clear();
    return retList;
}

void AbstractCommand::warnHelp(int exit_code, const QString& message)
{
    Log::get_logger()->critical(message);
    if(parser) parser->showHelp(exit_code);
}


#pragma clang diagnostic pop