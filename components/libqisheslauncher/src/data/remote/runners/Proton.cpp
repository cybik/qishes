//
// Created by cybik on 23-02-16.
//

#include <data/remote/runners/Proton.h>

#include <iostream>

#include <QNetworkAccessManager>
#include <QNetworkReply>

const std::string Proton::_proton_url = "https://raw.githubusercontent.com/cybik/yaagl-data/main/runners/proton.json";
std::shared_ptr<Proton> Proton::_self = nullptr;

std::shared_ptr<Proton> Proton::get_instance() {
    if(_self == nullptr) {
        _self.reset(new Proton());
    }
    return _self;
}

void Proton::handle(QNetworkReply* _rep) {
    parse_in(_rep->readAll());
    std::cout << to_string().toStdString() << std::endl;
    for(auto _el : getReleases()) {
        std::cout << _el.getName().toStdString() << std::endl << _el.getRelease().toStdString() << std::endl;
    }
}

void Proton::setups() {
    setup(QUrl(_proton_url.c_str()), [this](QNetworkReply* _reply) {
        this->handle(_reply);
    });
}

Proton::Proton() {
    setups();
}
