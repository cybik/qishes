//
// Created by cybik on 24-07-10.
//

#include <commands/DataCommand.h>
#include <QApplication>
#include <QCommandLineParser>
#include <QJsonArray>
#include <QJsonObject>

#include <common.h>
#include <iostream>
#include <qjsondocument.h>

//#include <XdgUtils/BaseDir/BaseDir.h>
#include <QStandardPaths>

#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <unistd.h>


const QString DataCommand::CommandSpecifier = "data";

int DataCommand::cmd_main(int argc, char **argv) {
    qwishes_data = std::make_shared<QApplication>(argc, argv);
    QApplication::setApplicationName(APPNAME_GEN(.data));
    QApplication::setApplicationVersion(APP_VERSION);

    parser = std::make_shared<QCommandLineParser>();
    parser->addHelpOption();
    parser->addVersionOption();

    parser->addPositionalArgument( "command", L18N_M("Command to run. MUST be data.") );

    std::shared_ptr<QCommandLineOption> game_path, file_path, known_url, all_targets;
    parser->addOption(
        *(game_path = std::make_shared<QCommandLineOption>(
            QStringList() << "g" << "game-path",
            L18N("Path to the game installation. May or may not be provided."),
            "game_path", nullptr
        ))
    );
    parser->addOption(
        *(file_path = std::make_shared<QCommandLineOption>(
            QStringList() << "f" << "file-path",
            L18N("Path to the cache file directly."),
            "file_path", nullptr
        ))
    );
    parser->addOption(
        *(known_url = std::make_shared<QCommandLineOption>(
            QStringList() << "u" << "known-url",
            L18N("Known URL. May or may not be provided."),
            "known_url_path", nullptr
        ))
    );
    parser->addOption(
        *(all_targets = std::make_shared<QCommandLineOption>(
            QStringList() << "a" << "all-targets",
            L18N("Get all Gacha targets."),
            "all_Targets", nullptr
        ))
    );
    parser->process(*qwishes_data);

    this->command_game_path   =   parser->value(*game_path);
    this->command_file_path   =   parser->value(*file_path);
    this->command_known_url   =   parser->value(*known_url);
    this->command_all_targets =   parser->isSet(*all_targets);   // if set, always true

    if(command_known_url.isEmpty() && command_game_path.isEmpty() && command_file_path.isEmpty())
        warnHelp(2, "No game path nor known URL was provided.\nOne of the two must be provided.");

    //std::shared_ptr<std::list<std::shared_ptr<QFile>>> caches;
    if(!this->command_file_path.isEmpty() && QFileInfo::exists(command_file_path)) {
        caches = std::make_shared<std::list<std::shared_ptr<QFile>>>();
        (*caches).emplace_front(std::make_shared<QFile>(QFileInfo(command_file_path).absoluteFilePath()));
    } else if(!this->command_game_path.isEmpty()) {
        caches = this->getGameWishesCache();
    } else
        warnHelp(5, "No good source of information was provided to extract a history URL from.");

    if(caches->empty()) warnHelp(3, "No URL was found in the detected cache.");
    printSingleFilePath((*caches).begin()->get()->fileName());

    mHttpClient = std::make_shared<HttpClient>();

    emit started();
    return qwishes_data->exec();
}

void DataCommand::started() {
    auto results = runFilterForLogs(runUrlCleanup(runUrlSearch(*(*caches).begin())));
    if(!results || results->empty()) warnHelp(4, "No URLs read, detected or otherwise found.");
    run_data_sync(results->front());
}

void DataCommand::run_data_sync(WishLog& log) {
    decode_initial_url(log);
    auto ret = mHttpClient->get_sync(log.getQuickInitUrl().toString());
    Log::get_logger()->critical(ret);
    start_sync_process(log, ret);
}

void DataCommand::decode_initial_url(WishLog& log) {
    std::cout << log.getQuickInitUrl().toString().toStdString() << std::endl;
}

void DataCommand::early_exit(const QString& message, int exit_code) {
    Log::get_logger()->warning(message);
    qwishes_data->exit(exit_code);
}

void DataCommand::check_initial_doc(QJsonDocument& doc)
{
    if( doc["data"].isUndefined() )                       early_exit("data absent", 10);
    if( doc["data"]["list"].isUndefined() )             early_exit("datalist absent", 11);
    if( doc["data"]["list"][0].isUndefined() )          early_exit("datalist element absent", 12);
    if( doc["data"]["list"][0]["uid"].isUndefined() ) early_exit("data element unexpected", 13);
}

void DataCommand::start_sync_process(WishLog& log, QByteArray result) {
    process_initial_data(log, nullptr, result);
    for(const auto& [key, value]: loaded_data) {
        qwishes_network_requests.emplace(
            key,
            std::make_shared<QNetworkAccessManager>(qwishes_data.get())
        );
        qwishes_network_requests[key]->setRedirectPolicy(QNetworkRequest::RedirectPolicy::ManualRedirectPolicy);
        run_sync_loop(log, key);
    }
}

// todo: add delays because too many requests. DoS detect.
void DataCommand::run_sync_loop(WishLog& log, const QString& key, int page) {
    Log::get_logger()->critical("Key: " + key);
    Log::get_logger()->warning("Value array count: " + QString::number(loaded_data[key].array().count()));
    Log::get_logger()->warning(loaded_data[key].array()[0].toObject().value("id").toString());
    Log::get_logger()->warning(loaded_data[key].array()[0].toObject().value("id").toString());

    QString target_url = log.regenerate_data_url(key.toInt(), page).toString();
    auto ret = mHttpClient->get_sync(target_url);
    Log::get_logger()->critical("First group clear! " + key + ": " + ret);
}

void DataCommand::process_initial_data(WishLog& log, QNetworkReply* reply, const QString& reply_as_str) {
    QJsonDocument doc = QJsonDocument::fromJson(reply==nullptr?reply_as_str.toLocal8Bit():reply->readAll());
    check_initial_doc(doc);

    // Generate data using first page UID.
    data_dir = QDir(
        std::filesystem::path(
                QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation)[0]
                    .toStdString()
            ).append(APPNAME_BASE)
            .append(get_local_storage_folder(log.game()))
            .append(doc["data"]["list"][0]["uid"].toString().toStdString()),
        "*.cache",
        QDir::SortFlags(QDir::NoSort),
        QDir::Filters(QDir::Files | QDir::NoDotAndDotDot)
    );
    if(!data_dir.exists()) {
        Log::get_logger()->warning("No pre-existing cache.");
        return;
    }
    for(const QFileInfo& file : data_dir.entryInfoList()) {
        QFile loaded(file.absoluteFilePath());
        loaded.open(QIODevice::ReadOnly);
        QString target_url = log.regenerate_data_url(file.baseName().toInt()).toString();
        loaded_data.emplace(file.baseName(),QJsonDocument::fromJson(loaded.readAll()));
        Log::get_logger()->warning("Loaded data from file: " + file.baseName());
        Log::get_logger()->info(target_url);
        //Log::get_logger()->critical(loaded_data[file.baseName()].toJson());
    }
    Log::get_logger()->info(data_dir.absolutePath());
}

std::string DataCommand::get_local_storage_folder(WishLog::WishLogGame game) {
    switch(game) {
        case WishLog::Genshin: return "yuanshen";
        case WishLog::HSR: return "hkrpg";
        case WishLog::ZZZ: return "nap";
        default: abort();
    }
}

/*
    https://gs.hoyoverse.com/nap/event/e20230424gacha/index.html?
        authkey_ver=1
        &sign_type=2
        &auth_appid=webview_gacha
        &win_mode=fullscreen
        &gacha_id=2c1f5692fdfbb733a08733f9eb69d32aed1d37
        &timestamp=1720620373
        &init_log_gacha_type=2001
        &init_log_gacha_base_type=2
        &ui_layout=
        &button_mode=default
        &plat_type=3
        &authkey=qEsfaOAGzSmE%2BfjT2l14NFp0K70%2Fd86qsRFNGhdkaGG6B5nAe00a%2FPZbTNgS0YvOYbdfUy9Ve%2BTxfGd0INMaTAE1%2Fwh3R9FcgpTAJqRypxokZ198SDQKDU3z%2B5JoZ%2FuT99LTTP1XeaG1wy3FT4XpDh9uCfqGYjecMejRCM7k2CcSb2tkVo%2F0bXweV9R%2FPD9eGrGLoTuiRmj%2Fjko4jDrB3nvmSpUnh3fqukoMmxpQiFdWP6V1VQlBOZSrZTGp%2FTGClsXOtlqq3UVlPYHZg8hutp5BJnEYD4Erw1mSDEc0l7CFB2COoTBoNpZaBXz3VaZK9wsuAA3hqSOBOk0VOI%2Bf%2B331EWIgCNDnQ0yDji%2BPOIw4rEcrwuiPPzVuQCxzN3X3OwsbG%2FhdFpwaTCiWAowBBeXNwYiZPoVc7%2BIKFwCIBF%2BHcmnfgPkzapMZGjeIfIJP2SxZtrCJ%2F7hI6owbrAcpULMtEVqrU4%2FjYlpGimr6ZeS3Hx8xCqTaJhq4s%2B7ZE6GYOBpMNpTKXk%2BjqfrcSLKnBQH6bXm4uLJpSPzhri%2FKrlKAE9cDZbUHx8EsaakFBCZCRyd68IAahF6wh%2BHs6fcllGk1foU3CEdjhWCqo4VRagDpJZDMdcl5VqslIe31U4YOY9OxgvdiXPoXoJUMZ8SnHhpBzSa25y5qZ6%2BSpVEA%2BR8%3D
        &lang=en
        &region=prod_gf_us
        &game_biz=nap_global#/info
    https://public-operation-nap-sg.hoyoverse.com/common/gacha_record/api/getGachaLog?
        ?authkey_ver=1
        &sign_type=2
        &auth_appid=webview_gacha
        &win_mode=fullscreen
        &gacha_id=2c1f5692fdfbb733a08733f9eb69d32aed1d37
        &timestamp=1720620373
        &init_log_gacha_type=2001
        &init_log_gacha_base_type=2
        &ui_layout=
        &button_mode=default
        &plat_type=3
        &authkey=qEsfaOAGzSmE%2BfjT2l14NFp0K70%2Fd86qsRFNGhdkaGG6B5nAe00a%2FPZbTNgS0YvOYbdfUy9Ve%2BTxfGd0INMaTAE1%2Fwh3R9FcgpTAJqRypxokZ198SDQKDU3z%2B5JoZ%2FuT99LTTP1XeaG1wy3FT4XpDh9uCfqGYjecMejRCM7k2CcSb2tkVo%2F0bXweV9R%2FPD9eGrGLoTuiRmj%2Fjko4jDrB3nvmSpUnh3fqukoMmxpQiFdWP6V1VQlBOZSrZTGp%2FTGClsXOtlqq3UVlPYHZg8hutp5BJnEYD4Erw1mSDEc0l7CFB2COoTBoNpZaBXz3VaZK9wsuAA3hqSOBOk0VOI%2Bf%2B331EWIgCNDnQ0yDji%2BPOIw4rEcrwuiPPzVuQCxzN3X3OwsbG%2FhdFpwaTCiWAowBBeXNwYiZPoVc7%2BIKFwCIBF%2BHcmnfgPkzapMZGjeIfIJP2SxZtrCJ%2F7hI6owbrAcpULMtEVqrU4%2FjYlpGimr6ZeS3Hx8xCqTaJhq4s%2B7ZE6GYOBpMNpTKXk%2BjqfrcSLKnBQH6bXm4uLJpSPzhri%2FKrlKAE9cDZbUHx8EsaakFBCZCRyd68IAahF6wh%2BHs6fcllGk1foU3CEdjhWCqo4VRagDpJZDMdcl5VqslIe31U4YOY9OxgvdiXPoXoJUMZ8SnHhpBzSa25y5qZ6%2BSpVEA%2BR8%3D
        &lang=en
        &region=prod_gf_us
        &game_biz=nap_global
        &page=1
        &size=10
        &gacha_type=2001
        &real_gacha_type=2
        &end_id=
*/

/*
    https://public-operation-hk4e-sg.hoyoverse.com/gacha_info/api/getGachaLog
    ?win_mode=fullscreen&authkey_ver=1&sign_type=2&auth_appid=webview_gacha
    &init_type=301
    &game_biz=hk4e_global
    &gacha_type=301
    &page=1
    &size=1
    &end_id=
    &gacha_id=4efc7dfdc6d6dfc22773a49663b1f5ca060b6ddf&timestamp=1721173233&lang=en
    &device_type=pc&game_version=OSRELWin4.8.0_R25683502_S25353456_D25722854&region=os_usa
    &authkey=GW2JPu5AetePHbO5YYC%2F89zxvXuF6USUDFkKS6gxSqzPRXnykyseD5BU1UecIMDFcERGwVvMMPwjT%2BfK9RKveCEKCYpKv%2BW1mthEwmDb0%2BG0UeTpncZ8jndo870r0pOoosuQ%2F3afHDvtvs3tEiGFNaelbg9%2B6Suhxt%2FEpJ%2FnVRxZr0p%2Bp%2FJqLSlmtkKsty8Vc8c16Y1GDWX89bUaJMNKq1XLTSybnO5WiFiZZyVSTd%2F2Q1sM%2FpJtpy%2BtGRWrZU9zUj8EFyOVj5A6tluyGYyVBNw06JqYvcA%2BYhoRtt6XHY82IqXowRfDzEhvDdDWGxBoB2x39O1QOX20OLfk3lXAU4ytf5WssJxP9Js8%2Fva4asIXm3027f7gyP3G6ymO2TGOlZMSxj%2FpEn0aFVx7rqh9LVax27QKQFLYwD%2FAgIbhmCtKAiGps8MQ31eTCeVHALKcddG2hcguDWRATlG18f0ur%2FDBFlV3emWX%2FrgKiuFzRzrTNwJ6B1mj%2Fq5XVdRWN1ZQ1x3pYL74qNxEDt7lMzkNx9AcaioCg4RN83zfCGm5wKIpVlwqeq7gnLFUf7rag82wvij1Tw9XHr%2BNH4V0L8RV%2BGzUTbJdk2eqZniME5LCAXwYyDG9FQNG5XZ3s2EDGN6Dff4SRBVxtFcVeAXPVbweg1Q560jQDdf1jwypqc7O5Sq7ijWOalONW6z%2BhMq3Y%2BsaD8dH0ep03NL3eewTpxSczwLeQ8hbp9nFQdgFdaX8q4x1ZHKpHt27GS6%2FBmUMqqghbwQsOuAgSJ%2BRyip5Mp5%2FoAwAKJWzc2NKr0obimZJQ70J7k0al%2BPKLHOcftkk2mTocPX4tsudiiH%2FTTXXLnJZo42YdgsozaxI1AUKxBi3hprd2MgnMnLHPg%2B8aujnX7T7SIAWv%2BR%2Frs8jaXGb15ioqfWzfwjoiPtXdKw3R4iQQ0NgoNJ8fUAS%2Ftr74DrPy51V3z%2B9iebgzPTed1z0oeBAb4bbNGe9T7ky%2FufHyTiS0bXy18AcrrVp0YrMiIGvMCfNp%2BQPon68bVHH357fJ6Fcwb2KEhOiQgX9UFu%2BXgXgU7FVht3eG7EyTrZ5uivvFcEVWT8Mwqt3rn%2FgCt5QQsZ%2FwqfWH3sh4hiruf5BTtk50%2BzmcWc%2BqZh0eJV6KLj5FAEOyXT6HIieuoYbsTqup7syqmqvTWc9ySVK%2F8ilMmemqU3z43%2B9AWOhmQo%2FiRGw9Iab6mBX%2F5xY3t8CNfQHHcuwuKUrSif%2Fr3RU5No2nv%2BX6HQbQRAk8B1eFdUFyj2fSEMw8r5sOHdUBSdoQin9d2sO207kZ8Nd8BkMrZJMQcrHcs%2BpgHlO6CIvDIFHV0kqrsjdJjSY%2FWz0C6zGkOamvqU1Ow%3D%3D
 */