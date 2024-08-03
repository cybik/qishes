//
// Created by cybik on 24-07-10.
//

#include <commands/DataCommand.h>
#include <QCoreApplication>
#include <QCommandLineParser>

#include <common.h>
#include <iostream>
#include <qjsondocument.h>

#include <XdgUtils/BaseDir/BaseDir.h>

#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <unistd.h>

const QString DataCommand::CommandSpecifier = "data";

void DataCommand::cmd_main(int argc, char **argv) {
    qwishes_data = new QCoreApplication(argc, argv);
    QCoreApplication::setApplicationName(APPLICATION_NAME_GENERATOR(.data));
    QCoreApplication::setApplicationVersion(APP_VERSION);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument(
        "command",
        QCoreApplication::translate("main", "Command to run. MUST be data.")
    );

    std::shared_ptr<QCommandLineOption> game_path, file_path, known_url, all_targets;
    parser.addOption(
        *(game_path = std::make_shared<QCommandLineOption>(
            QStringList() << "g" << "game-path",
            SPEC_TRANSLATE("Path to the game installation. May or may not be provided."),
            "game_path",
            nullptr
        ))
    );
    parser.addOption(
        *(file_path = std::make_shared<QCommandLineOption>(
            QStringList() << "f" << "file-path",
            SPEC_TRANSLATE("Path to the cache file directly."),
            "file_path",
            nullptr
        ))
    );
    parser.addOption(
        *(known_url = std::make_shared<QCommandLineOption>(
            QStringList() << "u" << "known-url",
            SPEC_TRANSLATE("Known URL. May or may not be provided."),
            "known_url_path",
            nullptr
        ))
    );
    parser.addOption(
        *(all_targets = std::make_shared<QCommandLineOption>(
            QStringList() << "a" << "all-targets",
            SPEC_TRANSLATE("Get all Gacha targets."),
            "all_Targets",
            nullptr
        ))
    );
    parser.process(*qwishes_data);

    this->command_game_path   =   parser.value(*game_path);
    this->command_file_path   =   parser.value(*file_path);
    this->command_known_url   =   parser.value(*known_url);
    this->command_all_targets =   parser.isSet(*all_targets);   // if set, always true

    if(command_known_url.isEmpty() && command_game_path.isEmpty() && command_file_path.isEmpty()) {
        Log::get_logger()->warning("No game path nor known URL was provided.");
        Log::get_logger()->warning("One of the two must be provided.");
        parser.showHelp(2);
    }

    std::shared_ptr<std::list<std::shared_ptr<QFile>>> caches;
    if(!this->command_file_path.isEmpty() && QFileInfo::exists(command_file_path)) {
        caches = std::make_shared<std::list<std::shared_ptr<QFile>>>();
        (*caches).emplace_front(std::make_shared<QFile>(QFileInfo(command_file_path).absoluteFilePath()));
    } else if(!this->command_game_path.isEmpty()) {
        caches = this->getGameWishesCache(&parser);
    } else {
        Log::get_logger()->critical("No good source of information was provided to extract a history URL from.");
        parser.showHelp(5);
    }

    if(caches->empty()) {
        Log::get_logger()->warning("No URL was found in the detected cache.");
        parser.showHelp(3);
    }
    printSingleFilePath((*caches).begin()->get()->fileName());

    auto results = runFilterForLogs(runUrlCleanup(runUrlSearch(*(*caches).begin())));
    if(!results || results->empty()) {
        Log::get_logger()->critical("No URLs read, detected or otherwise found.");
        parser.showHelp(4);
    }

    initialize(results->front());
    qwishes_data->exec();

}


void DataCommand::initialize(WishLog& log) {
    auto qnam = new QNetworkAccessManager(qwishes_data);
    qnam->setRedirectPolicy(QNetworkRequest::RedirectPolicy::ManualRedirectPolicy);
    qnam->get(QNetworkRequest(log.getQuickInitUrl()));
    std::cout << log.getQuickInitUrl().toString().toStdString() << std::endl;
    QObject::connect(
        qnam, &QNetworkAccessManager::finished,
        [&](QNetworkReply* reply) {
            if(reply->error() == QNetworkReply::NoError) {
                process_with_initial_data(log, reply);
            }
        }
    );
}

void DataCommand::process_with_initial_data(WishLog& log, QNetworkReply* reply) {
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    //std::cout << doc.toJson().toStdString() << std::endl;
    if( doc["data"].isUndefined() ) {
        abort();
    } else {
        std::cout << doc["data"].toString().toStdString() << std::endl;
        if(doc["data"]["list"].isUndefined() ) {
            abort();
        } else {
            if( doc["data"]["list"][0].isUndefined() ) {
                abort();
            } else {
                if( doc["data"]["list"][0]["uid"].isUndefined() ) {
                    abort();
                }
            }
        }
    }

    // Generate data using first page UID.
    data_dir = QDir(
        std::filesystem::path(XdgUtils::BaseDir::XdgDataHome())
            .append(APPLICATION_BASE_NAME)
            .append(get_local_storage_folder(log.game()))
            .append(doc["data"]["list"][0]["uid"].toString().toStdString())
    );
    load_local_data();
    Log::get_logger()->info(data_dir.absolutePath());
    qwishes_data->quit();
    abort();
}

void DataCommand::load_local_data() {
    QDir load_data_dir(data_dir);
    load_data_dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    load_data_dir.nameFilters() << "*.cache";
    if(load_data_dir.exists()) {
        for(const QFileInfo& file : load_data_dir.entryInfoList()) {
            QFile loaded(file.absoluteFilePath());
            loaded.open(QIODevice::ReadOnly);
            loaded_data.emplace(file.baseName(), QJsonDocument::fromJson(loaded.readAll()));
            Log::get_logger()->warning("Loaded data from file.");
            Log::get_logger()->critical(loaded_data[file.baseName()].toJson());

        }
    } else {
        Log::get_logger()->warning("No pre-existing cache.");
    }
}

std::string DataCommand::get_local_storage_folder(WishLog::WishLogGame game) {
    switch(game) {
        case WishLog::Genshin: return "yuanshen";
        case WishLog::HSR: return "hkrpg";
        case WishLog::ZZZ: return "nap";
        default:
            abort();
    }
}

/*
https://gs.hoyoverse.com/nap/event/e20230424gacha/index.html?
    authkey_ver=1&sign_type=2&auth_appid=webview_gacha&win_mode=fullscreen&gacha_id=2c1f5692fdfbb733a08733f9eb69d32aed1d37&timestamp=1720620373&init_log_gacha_type=2001&init_log_gacha_base_type=2&ui_layout=&button_mode=default&plat_type=3&authkey=qEsfaOAGzSmE%2BfjT2l14NFp0K70%2Fd86qsRFNGhdkaGG6B5nAe00a%2FPZbTNgS0YvOYbdfUy9Ve%2BTxfGd0INMaTAE1%2Fwh3R9FcgpTAJqRypxokZ198SDQKDU3z%2B5JoZ%2FuT99LTTP1XeaG1wy3FT4XpDh9uCfqGYjecMejRCM7k2CcSb2tkVo%2F0bXweV9R%2FPD9eGrGLoTuiRmj%2Fjko4jDrB3nvmSpUnh3fqukoMmxpQiFdWP6V1VQlBOZSrZTGp%2FTGClsXOtlqq3UVlPYHZg8hutp5BJnEYD4Erw1mSDEc0l7CFB2COoTBoNpZaBXz3VaZK9wsuAA3hqSOBOk0VOI%2Bf%2B331EWIgCNDnQ0yDji%2BPOIw4rEcrwuiPPzVuQCxzN3X3OwsbG%2FhdFpwaTCiWAowBBeXNwYiZPoVc7%2BIKFwCIBF%2BHcmnfgPkzapMZGjeIfIJP2SxZtrCJ%2F7hI6owbrAcpULMtEVqrU4%2FjYlpGimr6ZeS3Hx8xCqTaJhq4s%2B7ZE6GYOBpMNpTKXk%2BjqfrcSLKnBQH6bXm4uLJpSPzhri%2FKrlKAE9cDZbUHx8EsaakFBCZCRyd68IAahF6wh%2BHs6fcllGk1foU3CEdjhWCqo4VRagDpJZDMdcl5VqslIe31U4YOY9OxgvdiXPoXoJUMZ8SnHhpBzSa25y5qZ6%2BSpVEA%2BR8%3D&lang=en&region=prod_gf_us&game_biz=nap_global#/info
https://public-operation-nap-sg.hoyoverse.com/common/gacha_record/api/getGachaLog?
    authkey_ver=1&sign_type=2&auth_appid=webview_gacha&win_mode=fullscreen&gacha_id=2c1f5692fdfbb733a08733f9eb69d32aed1d37&timestamp=1720620373&init_log_gacha_type=2001&init_log_gacha_base_type=2&ui_layout=&button_mode=default&plat_type=3&authkey=qEsfaOAGzSmE%2BfjT2l14NFp0K70%2Fd86qsRFNGhdkaGG6B5nAe00a%2FPZbTNgS0YvOYbdfUy9Ve%2BTxfGd0INMaTAE1%2Fwh3R9FcgpTAJqRypxokZ198SDQKDU3z%2B5JoZ%2FuT99LTTP1XeaG1wy3FT4XpDh9uCfqGYjecMejRCM7k2CcSb2tkVo%2F0bXweV9R%2FPD9eGrGLoTuiRmj%2Fjko4jDrB3nvmSpUnh3fqukoMmxpQiFdWP6V1VQlBOZSrZTGp%2FTGClsXOtlqq3UVlPYHZg8hutp5BJnEYD4Erw1mSDEc0l7CFB2COoTBoNpZaBXz3VaZK9wsuAA3hqSOBOk0VOI%2Bf%2B331EWIgCNDnQ0yDji%2BPOIw4rEcrwuiPPzVuQCxzN3X3OwsbG%2FhdFpwaTCiWAowBBeXNwYiZPoVc7%2BIKFwCIBF%2BHcmnfgPkzapMZGjeIfIJP2SxZtrCJ%2F7hI6owbrAcpULMtEVqrU4%2FjYlpGimr6ZeS3Hx8xCqTaJhq4s%2B7ZE6GYOBpMNpTKXk%2BjqfrcSLKnBQH6bXm4uLJpSPzhri%2FKrlKAE9cDZbUHx8EsaakFBCZCRyd68IAahF6wh%2BHs6fcllGk1foU3CEdjhWCqo4VRagDpJZDMdcl5VqslIe31U4YOY9OxgvdiXPoXoJUMZ8SnHhpBzSa25y5qZ6%2BSpVEA%2BR8%3D&lang=en&region=prod_gf_us&game_biz=nap_global&page=1&size=10&gacha_type=2001&real_gacha_type=2&end_id=
*/