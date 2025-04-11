/******************************************************************
 *
 * none.h
 *
 * \brief None launch wrapper.
 *
 * No workarounds necessary, just launch the blasted thing.
 *
 * Created by cybik on 25-04-03, for qgachawishes.
 *
 ******************************************************************/


#pragma once

#include "workaround.h"


class NoneImpl : public AWorkaround {
public:
    NoneImpl(std::string executable) : target_executable(executable) {}
    void obtain() override {} // None is a noop.
    std::list<std::string> decorate() override; // probably the wrong function prototype
private:
    std::string target_executable;
};
