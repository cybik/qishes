/******************************************************************
 *
 * jadeite.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-03, for qgachawishes.
 *
 ******************************************************************/

#include "jadeite.h"
/**
 * Direct jq request to get the latest jadeite download.
 * curl -X 'GET' \
 *    'https://codeberg.org/api/v1/repos/mkrsym1/jadeite/releases/latest' \
 *    -H 'accept: application/json' | jq ".assets[0].browser_download_url"
 **/

#include <httpclient/httpclient.h>

#include <QJsonDocument>
#include <QByteArray>

#include <iostream>

std::vector<std::string> JadeiteImpl::decorate() {
    // TODO: get decorated call to actual game exe
    return {target_executable};
}

// TODO: have a list of mirrors to request against
const QString jadeite_release_rooturl = "https://codeberg.org/api/v1/repos/mkrsym1/jadeite/releases/latest";
const QString jadeite_versions_url = "https://codeberg.org/mkrsym1/jadeite/raw/branch/master/metadata.json";

void JadeiteImpl::obtain() {
    // TODO: get jadeite from remote, install into drive_c
    if (!http_client) {
        http_client = std::make_shared<HttpClient>();
    }
    QByteArray versions = http_client->get_sync(jadeite_versions_url);
    QJsonDocument qjson = QJsonDocument::fromJson(versions);
    std::cout << qjson["jadeite"]["version"].toString().toStdString() << std::endl;
}
