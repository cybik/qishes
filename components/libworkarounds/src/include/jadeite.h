/******************************************************************
 *
 * jadeite.h
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-03, for qgachawishes.
 *
 ******************************************************************/


#pragma once

/**
 * Direct jq request to get the latest jadeite download.
 * curl -X 'GET' \
 *    'https://codeberg.org/api/v1/repos/mkrsym1/jadeite/releases/latest' \
 *    -H 'accept: application/json' | jq ".assets[0].browser_download_url"
 **/

class jadeite {

};
