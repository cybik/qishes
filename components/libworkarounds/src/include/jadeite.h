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
/**
 * Direct jq request to get the latest jadeite download.
 * curl -X 'GET' \
 *    'https://codeberg.org/api/v1/repos/mkrsym1/jadeite/releases/latest' \
 *    -H 'accept: application/json' | jq ".assets[0].browser_download_url"
 **/

class JadeiteImpl : public AWorkaround {
public:
    JadeiteImpl(std::string executable) : target_executable(executable) {}
    void obtain() override; // get the bloody thing
    std::list<std::string> decorate() override; // probably the wrong function prototype
private:
    std::string target_executable;
};
