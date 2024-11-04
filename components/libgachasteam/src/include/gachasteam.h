//
// Created by cybik on 24-06-29.
//

#pragma once
#include <memory>

class gachasteam {
public:
    static std::shared_ptr<gachasteam> get_gachasteam_instance();
    bool running_under_steam();
protected:
private:
    gachasteam() = default;

    static std::shared_ptr<gachasteam> mInstance;
};
