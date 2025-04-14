/******************************************************************
 *
 * jadeite.h
 *
 * \brief Jadeite launch wrapper.
 *
 * Some games may require Jadeite to run. In such cases, use the
 * executable name and launch the game by wrapping it with Jadeite.
 * Obviously this also means making sure Jadeite is present and
 * usable where it is needed.
 *
 * Created by cybik on 25-04-03, for qgachawishes.
 *
 ******************************************************************/


#pragma once

#include "workaround.h"

#include <httpclient/httpclient.h>

class JadeiteImpl : public AWorkaround {
public:
    JadeiteImpl(std::string executable) : target_executable(executable) {}
    void obtain(std::string c_drive_dir) override; // get the bloody thing
    std::vector<std::string> decorate() override; // probably the wrong function prototype
private:
    std::string target_executable;
    std::shared_ptr<HttpClient> http_client;
};
