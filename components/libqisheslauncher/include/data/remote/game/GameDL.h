//
// Created by cybik on 23-02-26.
//

#ifndef QAGL_GAMEDL_H
#define QAGL_GAMEDL_H

#include <memory>

#include <data/remote/RemoteCommon.h>

class GameDL : protected RemoteCommon {
public:
    static std::shared_ptr<GameDL> getInstance();
private:
    GameDL();
    QString to_string();
    static std::shared_ptr<GameDL> _self;
protected:
    void handle(QNetworkReply*) override;
};


#endif //QAGL_GAMEDL_H
