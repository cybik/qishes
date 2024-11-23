//
// Created by cybik on 23-02-16.
//

#include <data/remote/runners/Wine.h>


#include <iostream>

#include <QNetworkAccessManager>
#include <QNetworkReply>

const std::string Wine::_wine_url = "https://raw.githubusercontent.com/cybik/yaagl-data/main/runners/wine.json";
std::shared_ptr<Wine> Wine::_self = nullptr;

std::shared_ptr<Wine> Wine::get_instance() {
    if(_self == nullptr) {
        _self.reset(new Wine());
    }
    return _self;
}

void Wine::handle(QNetworkReply* _rep) {
    parse_in(_rep->readAll());
    std::cout << to_string().toStdString() << std::endl;
    for(auto _el : getReleases()) {
        std::cout << _el.getName().toStdString() << std::endl << _el.getRelease().toStdString() << std::endl;
    }
}

void Wine::setups() {
    setup(QUrl(_wine_url.c_str()), [this](QNetworkReply* _reply) {
        this->handle(_reply);
    });
}

Wine::Wine() {
    setups();
}
