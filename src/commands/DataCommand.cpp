/******************************************************************
 *
 * DataCommand.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-07-10 for qgachawishes.
 *
 ******************************************************************/

#include <commands/DataCommand.h>
#include <QApplication>
#include <QCommandLineParser>
#include <QJsonArray>
#include <QJsonObject>

#include <common.h>
#include <iostream>
#include <qjsondocument.h>

#include <QStandardPaths>

#include <QNetworkReply>
#include <unistd.h>

#include <spinners.hpp>

const QString DataCommand::CommandSpecifier = "data";


void DataCommand::command_create_application(int& argc, char **argv) {
    qwishes_data = std::make_shared<QCoreApplication>(argc, argv);
    QApplication::setApplicationName(APPNAME_GEN(.data));
    QApplication::setApplicationVersion(APP_VERSION);
}

void DataCommand::command_setup_parser() {
    parser->addPositionalArgument(
        "command",
        L18N_M("Command to run. MUST be data.")
    );

    parser->addOption(
        *(verbose = std::make_shared<QCommandLineOption>(
            QStringList() << "verbose",
            L18N("Be verbose about operations.")
        ))
    );
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
            L18N("Get all Gacha targets.")
        ))
    );
}

void DataCommand::command_process_parser() {
    parser->process(*qwishes_data);

    this->command_game_path   =   parser->value(*game_path);
    this->command_file_path   =   parser->value(*file_path);
    this->command_known_url   =   parser->value(*known_url);
    this->command_all_targets =   parser->isSet(*all_targets);   // if set, always true
    this->command_verbose     =   parser->isSet(*all_targets);   // if set, always true

    if(command_known_url.isEmpty() && command_game_path.isEmpty() && command_file_path.isEmpty())
        warnHelp(2, "No game path nor known URL was provided.\nOne of the two must be provided.");

    if( this->command_verbose ) Log::get_logger()->log_level(Log::Info);
}

std::shared_ptr<HttpClient> DataCommand::get_http_client() {
    if(!mHttpClient)
        mHttpClient = std::make_shared<HttpClient>();
    return mHttpClient;
}

int DataCommand::command_run() {
    if(!this->command_file_path.isEmpty() && QFileInfo::exists(command_file_path)) {
        caches = std::make_shared<std::list<std::shared_ptr<QFile>>>();
        (*caches).emplace_front(std::make_shared<QFile>(QFileInfo(command_file_path).absoluteFilePath()));
    } else if(!this->command_game_path.isEmpty()) {
        caches = this->getGameWishesCache();
    } else {
        warnHelp(5, "No good source of information was provided to extract a history URL from.");
    }

    if(caches->empty()) warnHelp(3, "No URL was found in the detected cache.");
    printSingleFilePath((*caches).begin()->get()->fileName());

    started();
    return 0;
}

void DataCommand::started() {
    auto results = runFilterForLogs(runUrlCleanup(runUrlSearch(*(*caches).begin())));
    if(!results || results->empty()) warnHelp(4, "No URLs read, detected or otherwise found.");
    run_data_sync(results->front());
}

void DataCommand::run_data_sync(WishLog& log) {
    Log::get_logger()->debug(log.getQuickInitUrl().toString(QUrl::EncodeSpaces));
    auto ret = get_http_client()->get_sync(log.getQuickInitUrl().toDisplayString());
    Log::get_logger()->info("Game identified as " + log.get_identified_game_name());
    start_sync_process(log, ret);
}

void DataCommand::early_exit(const QString& message, int exit_code) {
    Log::get_logger()->warning(message);
    qwishes_data->exit(exit_code);
}

void DataCommand::exit_check(bool condition, const QString& message, int exit_code) {
    if( condition ) early_exit(message, exit_code);
}

void DataCommand::check_initial_doc(QJsonDocument& doc)
{
    exit_check(doc["data"].isUndefined(), "data absent", 10);
    exit_check(doc["data"]["list"].isUndefined(), "datalist absent", 11);
    exit_check(doc["data"]["list"][0].isUndefined(), "datalist element absent", 12);
    exit_check(doc["data"]["list"][0]["uid"].isUndefined(), "data unexpected", 13);
}

void DataCommand::start_sync_process(WishLog& log, QByteArray result) {
    jms::Spinner lSpinner("Processing initial data", jms::dots);
    lSpinner.start();
    process_initial_data(log, nullptr, result);
    lSpinner.finish(jms::FinishedState::SUCCESS, "Initial data loaded!");
    for(const auto& key: get_pull_id_list(log.game())) {
        if(!loaded_data.contains(key)) {
            // New cache
            loaded_data.emplace(key, QJsonDocument(QJsonArray()));
        }
        lSpinner.setText("Processing data for " + key.toStdString());
        lSpinner.start();
        sleep(1);

        auto sync_result = sync_loop(log, key);
        auto initial_count = loaded_data[key].array().count();

        // In the specific case where we know the last one has matched.
        // This is the cleanest data unification we can possibly have.
        if( !sync_result->isEmpty()
            && (
                loaded_data[key].array().first().toObject()["id"].toString()
                    == sync_result->last().toObject()["id"].toString()
            )
        ) sync_result->removeLast();

        // Only do the switcheroo if there's anything to add.
        if(!sync_result->isEmpty()) {
            // Qt's QJsonDocument is read-only so we have to play with the data *outside*.
            QJsonArray existing = QJsonArray(loaded_data[key].array());
            while(!sync_result->isEmpty()) {
                existing.push_front(sync_result->last().toObject());
                sync_result->removeLast();
            }
            loaded_data[key].setArray(existing);
        }
        if(initial_count < loaded_data[key].array().count()) write_back(key); // only write back if there's changes

        lSpinner.finish(jms::FinishedState::SUCCESS, key.toStdString() + " is done!");
    }
    lSpinner.start();
    lSpinner.finish(jms::FinishedState::SUCCESS, "All done!");
}

void DataCommand::write_back(const QString& key) {
    if(data_dir.exists(QString(key).append(".cache"))) {
        // mv
        QFile(
            data_dir.filePath(QString(key).append(".cache"))
        ).rename(
            data_dir.filePath(
                QString(key)
                    .append(".cache.")
                    .append(QString::number( QDateTime::currentSecsSinceEpoch() ))
            )
        );
    }
    QFile write_back(data_dir.filePath(QString(key).append(".cache")));
    write_back.open(QIODevice::ReadWrite);
    write_back.write(loaded_data[key].toJson());
    write_back.close(); // clean up after ourselves. Good habits and all.
}

std::tuple<int, std::shared_ptr<QJsonDocument>>
DataCommand::get_sync_json(const QString& target_url) {
    if(QJsonDocument ret = QJsonDocument::fromJson(get_http_client()->get_sync(target_url));
        !ret.isEmpty() && ( !( ret["retcode"].isUndefined() || ret["data"].isUndefined() ) )
    ) {
        return {
            ret["retcode"].toInt(-1),
            std::make_shared<QJsonDocument>(ret["data"].toObject())
        };
    }
    return { -1, nullptr };
}

std::tuple<bool, std::shared_ptr<QJsonValue>>
DataCommand::is_latest_id_in_incoming(const QString& latest, std::shared_ptr<QJsonDocument> doc) {
    // doc is the whole doc.
    for(auto array_element: (*doc)["list"].toArray()) {
        if( array_element.toObject().contains("id")
            && array_element.toObject()["id"].toString().compare(latest) == 0
        ) {
            return { true, nullptr };
        }
    }
    return { false, std::make_shared<QJsonValue>(doc->object().value("list").toArray().last()) };
}

QString DataCommand::get_latest_id_from_key(const QString& key) {
    return loaded_data[key][0]["id"].toString();
}

bool DataCommand::write_condition_for_cache(const QString& key, const QString& incoming_id) {
    return ((loaded_data[key].array().count() == 0)? true : (incoming_id != get_latest_id_from_key(key)));
}

// todo: add delays because too many requests. DoS detect.
std::unique_ptr<QJsonArray>
DataCommand::sync_loop(WishLog& log, const QString& key, int page, std::shared_ptr<QJsonValue> id_val) {
    if(page > 1) sleep(2); // anti-ddos

    QString target_url = log.regenerate_data_url(
        key.toInt(),
        page,
        (id_val == nullptr ? "" : id_val->toObject()["id"].toString())
    ).toString();
    if(this->command_verbose) Log::get_logger()->info("Target URL to consult: " + target_url);
    const auto [retcode, data] = get_sync_json(target_url);
    const auto [is_latest, id_list]
        = is_latest_id_in_incoming(get_latest_id_from_key(key), data);
    auto ret_arr = QJsonArray((*data)["list"].toArray());
    bool is_empty = (page <= 1 && (*data)["list"].toArray().count() == 0);
    if(!is_empty && !is_latest) {
        for(auto loop = sync_loop(log, key, page + 1, id_list); auto el: *loop) {
            ret_arr.append(QJsonObject(el.toObject()));
            if(el.toObject()["id"].toString().compare(get_latest_id_from_key(key)) == 0) break;
        }
        return std::move(std::make_unique<QJsonArray>(ret_arr));
    }

    // When we get here, we have the "last" / sum total sequence, and we want to clean up the list's tail so
    //  we *end* with the expected end_id.
    // TODO: this assume regular syncs and a matching end_id. THIS DOES NOT SUPPORT "INITIAL DATA"
    //  OR TOO-LONG-DIDN'T-SYNC-MY-SHIT.
    if(!is_empty)
        while(write_condition_for_cache(key, ret_arr.last().toObject().value("id").toString()))
            ret_arr.removeLast();

    return std::move(std::make_unique<QJsonArray>(ret_arr));
}

void DataCommand::process_initial_data(WishLog& log, QNetworkReply* reply, const QString& reply_as_str) {
    QJsonDocument doc = QJsonDocument::fromJson(reply==nullptr?reply_as_str.toLocal8Bit():reply->readAll());
    check_initial_doc(doc);

    // Generate data using first page UID.
    data_dir = QDir(
        std::filesystem::path(
            QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation)[0]
                .toStdString()
            )
            .append(APPNAME_BASE)
            .append(get_local_storage_folder(log.game()))
            .append(doc["data"]["list"][0]["uid"].toString().toStdString()),
        "*.cache",
        QDir::SortFlags(QDir::NoSort),
        QDir::Filters(QDir::Files | QDir::NoDotAndDotDot)
    );
    if(!data_dir.exists()) {
        Log::get_logger()->warning("No pre-existing cache in " + data_dir.absolutePath());
        return;
    }
    for(const QFileInfo& file : data_dir.entryInfoList()) {
        QFile loaded(file.absoluteFilePath());
        loaded.open(QIODevice::ReadOnly);
        loaded_data.emplace(file.baseName(),QJsonDocument::fromJson(loaded.readAll()));
        loaded.close();
    }
}

std::string DataCommand::get_local_storage_folder(WishLog::WishLogGame game) {
    switch(game) {
        case WishLog::Genshin: return "yuanshen";
        case WishLog::HSR:     return "hkrpg";
        case WishLog::ZZZ:     return "nap";
        default: abort();
    }
}

std::list<QString> DataCommand::get_pull_id_list(WishLog::WishLogGame game) {
    switch(game) {
    case WishLog::Genshin: return {"200", "301", "302", "500" };
    case WishLog::HSR:     return {"1", "2", "11", "12" };
    //case WishLog::ZZZ:     return {"nap"};
    default: abort();
    }

}