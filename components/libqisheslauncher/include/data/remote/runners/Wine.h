//
// Created by cybik on 23-02-16.
//

#ifndef QAGL_WINE_H
#define QAGL_WINE_H

#include <string>
#include <memory>

#include "RunnerCommon.h"

class Wine : public RunnerCommon {
public:
    static std::shared_ptr<Wine> get_instance();
protected:
    void handle(QNetworkReply*) override;
private:
    Wine();
    static const std::string _wine_url;

    static std::shared_ptr<Wine> _self;

    void setups();

    std::string _identifier;
};


#endif //QAGL_WINE_H
