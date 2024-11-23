//
// Created by cybik on 23-02-16.
//

#ifndef QAGL_PROTON_H
#define QAGL_PROTON_H

#include <string>
#include <memory>

#include "RunnerCommon.h"

class Proton : public RunnerCommon {
public:
    static std::shared_ptr<Proton> get_instance();
protected:
    void handle(QNetworkReply*) override;
private:
    Proton();
    static const std::string _proton_url;

    static std::shared_ptr<Proton> _self;

    void setups();

    static const std::string _identifier;
};


#endif //QAGL_PROTON_H
