//
// Created by cybik on 22-02-13.
//

#include <ui/Landing.h>
#include <util/Constants.h>
#include <resources/stylesheets.h>
#include <resources/SASSProcess.h>
#include <resources/inject.h>

#include <QVBoxLayout>
#include <QWebEngineScript>
#include <QShortcut>

#define YAAGL_SETTINGS "#yaagl-settings"

#include <iostream>

#if 0
void examine(const QJsonObject& object) {
    std::cout << "---- gap ----" << std::endl;
    for(const QString& ref: object.keys()) {
        std::cout << ref.toStdString() << " :: " << object.find(ref).value().toString().toStdString() << std::endl;
    }
}
#endif

namespace QAGL {
    void Landing::show_dev() {
        if(!devTools_Window) {
            devTools_Window = std::make_shared<QMainWindow>();
            devTools_Window->setFixedSize(1920, 1080);
            devTools_WebEngine = std::make_shared<QWebEngineView>();
            launcher_WebEngine->page()->setInspectedPage(launcher_WebEngine->page());
            launcher_WebEngine->page()->setDevToolsPage(devTools_WebEngine->page());
            devTools_Window->setCentralWidget(devTools_WebEngine.get());
        }
        devTools_Window->show();
    }

    QString Landing::bg_gamebiz() {
        QString bg_endswith = (
            _region == QAGL_Region::cn
            ? "cn"
            : "global"
        );
        switch (_game) {
        case h4ke: return ("hk4e_" + bg_endswith);
        case nap: return ("nap_" + bg_endswith);
        case bh3: return ("bh3_" + bg_endswith);
        case hkrpg: return ("hkrpg_" + bg_endswith);
        }
    }

    void Landing::background_set() {
        QString back = "";
        // TODO: game selection. Initial: support for hk4e only
        if (!background->isNull()
            && (*background)["data"].isObject()
            && (*background)["data"]["game_info_list"].isArray()
        ) {
            for (auto elem: (*background)["data"]["game_info_list"].toArray()) {
                if (!elem.isUndefined()
                    && elem.toObject()["game"].isObject()
                    && elem.toObject()["backgrounds"].isArray()
                ) {
                    if (elem.toObject()["game"].toObject()["biz"].isString()) {
                        if (elem.toObject()["game"].toObject()["biz"].toString().compare(bg_gamebiz()) == 0) {
                            back =
                                elem.toObject()["backgrounds"]
                                    .toArray().at(0).toObject()["background"]
                                    .toObject()["url"].toString();
                            break;
                        }
                    }
                }
            }
        }
        /*
        QString back = background->object().find("data").value().toObject()
                .find("adv").value().toObject()
                .find("background").value().toString();*/
        if (!back.isEmpty()) {
            launcher_WebEngine->page()->runJavaScript(
                "document.body.background = ('"+back+"');",
                [this](const QVariant& var) {
                    networkRequest.reset();
                    launcher_Window->show();
                }
            );
        } else {
            launcher_Window->show();
        }
    }

    // Final method after getting background URL, load up
    void Landing::background_req(QNetworkReply *reply) {
        background = std::make_shared<QJsonDocument>(QJsonDocument::fromJson(reply->readAll()));
        //std::cout << reply->url().toString().toStdString() << std::endl;
        background_set();
    }

    void Landing::runBackground() {
        if(!background) {
            if(networkLink == nullptr) {
                networkLink = std::make_shared<QNetworkAccessManager>();
                QObject::connect(
                    networkLink.get(), SIGNAL(finished(QNetworkReply *)),
                    this, SLOT(background_req(QNetworkReply*))
                );
            }
            //networkRequest = std::make_shared<QNetworkRequest>(QUrl((backgroundUri + "en-us").c_str()));
            networkRequest = std::make_shared<QNetworkRequest>(QUrl((backgroundUri_hyp + "en-us").c_str()));
            networkLink->get(*networkRequest);
            return;
        }
        background_set();
    }

    void Landing::loaded(bool is) {
        if(is) {
            launcher_WebEngine->page()->runJavaScript(
                "document.getElementsByClassName('home')[0].clientWidth;",
                [this](const QVariant& var) {
                    launcher_Window->setFixedWidth(var.toInt());
                    launcher_WebEngine->page()->runJavaScript(
                        "document.getElementsByClassName('home')[0].clientHeight;",
                        [this](const QVariant& var) {
                            launcher_Window->setFixedHeight(var.toInt());
                            launcher_Window->move(
                                QGuiApplication::primaryScreen()->geometry().center() - launcher_Window->rect().center()
                            );
                            runBackground();
                        }
                    );
                }
            );

            {
                if (networkLink_data == nullptr) {
                    networkLink_data = std::make_shared<QNetworkAccessManager>();
                    QObject::connect(
                        networkLink_data.get(), &QNetworkAccessManager::finished,
                        [](QNetworkReply *reply) {
                            auto in_data = std::make_shared<QJsonDocument>(
                                QJsonDocument::fromJson(reply->readAll())
                            );
                            //std::cout << in_data->toJson().toStdString() << std::endl;
                        }
                    );
                }
                networkRequest_data = std::make_shared<QNetworkRequest>(QUrl((versionUri + "en-us").c_str()));
                //std::cout << (versionUri + "en-us") << std::endl;
                networkLink_data->get(*networkRequest_data);
            }
            return;
        }
    }

    void Landing::inject_stylesheet() {
        if(launcher_WebEngine->page()->scripts().isEmpty() ||
           launcher_WebEngine->page()->scripts().find("cpp-sends-their-regards").empty()
        ) {
            QWebEngineScript script;
            QString s = QString::fromLatin1(QAGL_INJECT_STYLESHEET)
                            .arg("cpp-sends-their-regards")
                            .arg(SASSProcess(idx_sass.toStdString()).c_str());
            launcher_WebEngine->page()->runJavaScript(s, QWebEngineScript::ApplicationWorld);

            script.setName("cpp-sends-their-regards");
            script.setSourceCode(s);
            script.setInjectionPoint(QWebEngineScript::DocumentReady);
            script.setRunsOnSubFrames(true);
            script.setWorldId(QWebEngineScript::ApplicationWorld);
            launcher_WebEngine->page()->scripts().insert(script);
        }
    }

    void Landing::inject_settings() {
        if(launcher_WebEngine->page()->scripts().isEmpty() ||
           launcher_WebEngine->page()->scripts().find("setts").empty()
        ) {
            QWebEngineScript script;
            QString s = QString::fromLatin1(QAGL_INJECT_SETTINGS)
                            .arg(gear_idle.c_str())
                            .arg(gear_hover.c_str())
                            .arg(YAAGL_SETTINGS);
            launcher_WebEngine->page()->runJavaScript(s, QWebEngineScript::ApplicationWorld);

            script.setName("setts");
            script.setSourceCode(s);
            script.setInjectionPoint(QWebEngineScript::DocumentReady);
            script.setRunsOnSubFrames(true);
            script.setWorldId(QWebEngineScript::ApplicationWorld);
            launcher_WebEngine->page()->scripts().insert(script);
        }
    }

    Landing::Landing(
        const QApplication &app, std::shared_ptr<SettingsData> settings, QAGL::QAGL_App_Style style,
        QAGL::QAGL_Game game, QAGL::QAGL_Region region
    ) {
        _game = game;
        _region = region;
        _configData = settings;

        launcher_Window = std::make_shared<QMainWindow>();

        // defaults
        launcher_Window->setFixedSize(1280, 730);
        //launcher_Window->setFixedSize(1280, 720 - QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight));
        launcher_Window->setWindowTitle("Yet Another Anime Game Launcher");

        // Menu
        devTools_Combo = std::make_shared<QShortcut>(QKeySequence(Qt::Key_F12), launcher_Window.get());
        QObject::connect(
            devTools_Combo.get(), SIGNAL(activated()),
            this, SLOT(show_dev())
        );

        // Web core
        launcher_WebEngine = std::make_shared<QWebEngineView>();
        launcher_WebEngine->setContextMenuPolicy(Qt::NoContextMenu);
        launcher_WebEngine->setAcceptDrops(false);
        launcher_WebPage = std::make_shared<QAGL::LandingWebEnginePage>();
        launcher_WebPage->setParentWindow(launcher_Window);
        launcher_WebEngine->setPage(launcher_WebPage.get());
        inject_stylesheet();
        inject_settings();
        QObject::connect(
            launcher_WebEngine.get(), SIGNAL(loadFinished(bool)),
            this, SLOT(loaded(bool))
        );

        // Add the web core to the window
        launcher_WidgetStack = new QStackedWidget(nullptr);
        launcher_WidgetStack->addWidget(launcher_WebEngine.get());
        _style = style;
        if(style == QAGL_App_Style::Unique_Window) {
            /*
            launcher_WidgetStack->addWidget(createSettings()->getWidget()); // first
            createSettings()->setExitCallback(
                [&]() {
                    exit_settings();
                }
            );*/
        }
        launcher_Window->setCentralWidget(launcher_WidgetStack);
    }
    /*
    void Landing::exit_settings() {
        launcher_WidgetStack->setCurrentIndex(0);
    }

    void Landing::load_settings() {
        createSettings()->showing();
        if(_style == QAGL_App_Style::Normal) {
            createSettings()->show();
        } else {
            launcher_WidgetStack->setCurrentIndex(1);
        }
    }*/

    /*
    std::shared_ptr<SettingsWindow> Landing::createSettings() {
        if(settings == nullptr) {
            settings = std::make_shared<SettingsWindow>(_style == QAGL_App_Style::Unique_Window);
            settings->setConfig(_configData->getSettings());
        }
        return settings;
    }
    */

    QString Landing::generate_url() {
        auto ret = QString("https://")
                .append(placeholders::lowercase::first.c_str()).append(".")
                .append(placeholders::lowercase::company_os.c_str())
                .append(".com/launcher/10/en-us?api_url=https%3A%2F%2Fapi-os-takumi.")
                .append(placeholders::lowercase::company.c_str())
                .append(".com%2Fhk4e_global&key=gcStgarh&prev=false");
        //std::cout << ret.toStdString() << std::endl;
        return ret;
    }

    void Landing::show(const QApplication &app) {
        launcher_WebEngine->load(QUrl(generate_url()));
    }

    QWebEnginePage * LandingWebEnginePage::createWindow(WebWindowType type) {
        return new LandingWebEnginePage();
    }

    bool LandingWebEnginePage::acceptNavigationRequest(const QUrl& url, NavigationType _type, bool _ignore) {
        switch (_type) {
            case(QWebEnginePage::NavigationTypeLinkClicked): {
                QDesktopServices::openUrl(url);
                return false;
            }
            case (QWebEnginePage::NavigationTypeRedirect): {
                if (url.toString().contains(YAAGL_SETTINGS)) {
                    qDebug() << "WIP: Settings";
                    _parentSettings();
                    return false;
                }
                break;
            }
            default: { break; }
        }
        return true;
    }

    /*
    LandingWebEnginePage* LandingWebEnginePage::setSettingsLambda(std::function<void()> lambda) {
        _parentSettings = std::move(lambda);
        return this;
    }
    */

    void LandingWebEnginePage::setParentWindow(std::shared_ptr<QMainWindow> ptr) {
        _parent = ptr;
        //return this;
    }
}
