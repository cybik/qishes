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
#include <QDir>

#include <iostream>

#include <termcolor/termcolor.hpp>
#include <qmicroz.h>

std::vector<std::string> JadeiteImpl::decorate() {
    // TODO: get decorated call to actual game exe
    if (jadeite_active) {
        return { calculated_jadeite_path.toStdString(), target_executable };
    }
    return {target_executable};
}

// TODO: have a list of mirrors to request against
const QString jadeite_release_rooturl = "https://codeberg.org/api/v1/repos/mkrsym1/jadeite/releases/latest";
const QString jadeite_versions_url = "https://codeberg.org/mkrsym1/jadeite/raw/branch/master/metadata.json";

// 	"https://codeberg.org/mkrsym1/jadeite/releases/download/v5.0.1/v5.0.1.zip"

void JadeiteImpl::obtain(std::string c_drive_dir) {
    // TODO: get jadeite from remote, install into drive_c
    if (!http_client) {
        http_client = std::make_shared<HttpClient>();
    }
    QByteArray versions = http_client->get_sync(jadeite_versions_url);
    QJsonDocument qjson = QJsonDocument::fromJson(versions);
    QString version = qjson["jadeite"]["version"].toString();
    QString processed_url = "https://codeberg.org/mkrsym1/jadeite/releases/download/v"+version+"/v"+version+".zip";
    std::cout
        << termcolor::on_bright_yellow
            << "Processed URL:" << processed_url.toStdString()
            << termcolor::reset
    << std::endl;

    QDir fs_jadeite_dir = QDir((c_drive_dir + "/Jadeite").c_str());
    QString out_filename = processed_url.split('/').last();
    QDir jadeite_unpack = QDir(fs_jadeite_dir.filesystemPath().append(version.toStdString()));
    QFile jadeite_archive = QFile(fs_jadeite_dir.filesystemPath().append(out_filename.toStdString()));

    // guessed paths
    local_jadeite_dir = jadeite_unpack.absolutePath();

    // reprocess
    relative_jadeite_dir = local_jadeite_dir;
    relative_jadeite_dir.replace(c_drive_dir.c_str(), "");
    relative_jadeite_dir_wine_variant = relative_jadeite_dir;
    relative_jadeite_dir_wine_variant.replace("/", "\\");
    local_jadeite_version = version;

    calculated_jadeite_path = jadeite_unpack.filesystemPath().append("jadeite.exe").c_str();
    std::cout
        << termcolor::green
            << "Guessed information:" << std::endl
                << "\t" << "absolute file\t\t\t"         << calculated_jadeite_path.toStdString() << std::endl
                << "\t" << "absolute dir\t\t\t"          << local_jadeite_dir.toStdString() << std::endl
                << "\t" << "relative dir\t\t\t"          << relative_jadeite_dir.toStdString() << std::endl
                << "\t" << "relative dir (wine)\t\t"     << relative_jadeite_dir_wine_variant.toStdString() << std::endl
                << "\t" << "jadeite version\t\t\t"       << local_jadeite_version.toStdString() << std::endl
            << termcolor::reset
    << std::endl;

    if (fs_jadeite_dir.exists()) {
        // Case 1: exists. Check if file we'd queue for download exists; if so, just skip.
        if ( (jadeite_archive.exists() || jadeite_unpack.exists()) && QFile(calculated_jadeite_path).exists() ) {
            std::cout
                << termcolor::on_bright_yellow
                    << "Jadeite already downloaded"
                    << termcolor::reset
            << std::endl;
            jadeite_active = true;
            return;
        }
    } else {
        if (!std::filesystem::exists(fs_jadeite_dir.filesystemPath().parent_path())) {
            // The PARENT path Jadeite doesn't exist. This fucked.
            std::cout
                << termcolor::on_bright_red << termcolor::blink
                    << "The folder structure is broken. SOMETHING VERY FUCKED."
                    << termcolor::reset
            << std::endl;
            return;
        }
        fs_jadeite_dir.mkpath((c_drive_dir + "/Jadeite").c_str());
    }
    jadeite_archive.open(QIODeviceBase::NewOnly|QIODeviceBase::WriteOnly);
    jadeite_archive.write(http_client->get_sync(processed_url));

    // And extract!
    QMicroz::extract(
        absolute(jadeite_archive.filesystemFileName()).c_str(),
        jadeite_unpack.absolutePath()
    );
    jadeite_active = true;
}
