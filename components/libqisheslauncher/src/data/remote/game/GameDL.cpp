//
// Created by cybik on 23-02-26.
//

#include <data/remote/game/GameDL.h>

#include <util/Constants.h>

#include <iostream>

std::shared_ptr<GameDL> GameDL::_self = nullptr;

GameDL::GameDL() {
    this->setup(QUrl(QAGL::versionUri.c_str()),  [this](QNetworkReply* _reply) {
        this->handle(_reply);
    });
}

std::shared_ptr<GameDL> GameDL::getInstance() {
    if(!_self) {
        _self.reset(new GameDL());
    }
    return _self;
}

QString GameDL::to_string() {
    // noop
}

void GameDL::handle(QNetworkReply * _rep) {
    std::cout << _rep->readAll().toStdString() << std::endl;
}
