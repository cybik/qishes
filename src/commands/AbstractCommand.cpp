//
// Created by cybik on 24-06-27.
//

#include <commands/AbstractCommand.h>
#include <iostream>

#import <gachafs.h>

const QString filter = "**/webCaches/**/Cache/Cache_Data/data_2";

std::shared_ptr<std::list<std::shared_ptr<QFile>>> AbstractCommand::getGameWishesCache() {
    return gachafs::getFiles(filter, this->command_game_path);
}