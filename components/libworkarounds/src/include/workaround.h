/******************************************************************
 *
 * workarounds.h
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-03, for qgachawishes.
 *
 ******************************************************************/

#pragma once

#include <string>
#include <memory>
#include <vector>

class AWorkaround {
public:
    virtual void obtain() = 0;
    virtual std::vector<std::string> decorate() = 0;
};

class Workaround {
public:
    typedef enum {
        Jadeite,
        None
    } Handler;
    static std::unique_ptr<AWorkaround> getWorkaround(Handler handler, std::string executable);
};
