//
// Created by cybik on 22-02-13.
//

#ifndef QAGL_LANDING_H
#define QAGL_LANDING_H

#include <resources/gear_image.h>
#include <QMainWindow>
#include <QStackedWidget>
#include <QtWebEngineWidgets/QtWebEngineWidgets>

#include <memory>

#include <functional>
//#include "settings/SettingsWindow.h"

#include <data/SettingsData.h>

namespace QAGL {
    typedef enum {
        Normal,
        Unique_Window
    } QAGL_App_Style;
    class Landing : public QObject {
Q_OBJECT
    public:
        explicit Landing(const QApplication &app,
                         std::shared_ptr<SettingsData> settings,
                         QAGL_App_Style style = QAGL_App_Style::Normal
        );
        void show(const QApplication &app);

    public slots:
        void background_req(QNetworkReply *);
        void show_dev();
        //void exit_settings();
        void loaded(bool);

    protected:
    private:
        Landing() = default;
        void inject_stylesheet();
        void inject_settings();
        void runBackground();

        //void load_settings();
        //std::shared_ptr<SettingsWindow> settings;

        // Launcher Landing
        std::shared_ptr<QMainWindow> launcher_Window;
        QStackedWidget* launcher_WidgetStack;
        std::shared_ptr<QWebEngineView> launcher_WebEngine;

        // Webkit/Chromium Developer Console
        std::shared_ptr<QMainWindow> devTools_Window;
        std::shared_ptr<QWebEngineView> devTools_WebEngine;
        std::shared_ptr<QShortcut> devTools_Combo;

        // Network requests to our gaming overlords
        std::shared_ptr<QNetworkAccessManager> networkLink;
        std::shared_ptr<QNetworkAccessManager> networkLink_data;
        std::shared_ptr<QNetworkRequest> networkRequest;
        std::shared_ptr<QNetworkRequest> networkRequest_data;

        // JSON - Background Image data
        std::shared_ptr<QJsonDocument> background;

        std::shared_ptr<SettingsData> _configData;

        //std::shared_ptr<SettingsWindow> createSettings();

        // Utilities
        static QString generate_url();
        void background_set();

        QAGL_App_Style _style = QAGL_App_Style::Normal;
    };

    class LandingWebEnginePage : public QWebEnginePage
    {
Q_OBJECT
    public:
        explicit LandingWebEnginePage(QObject* parent = 0) : QWebEnginePage(parent) { }
        LandingWebEnginePage *setSettingsLambda(std::function<void()>);
        LandingWebEnginePage *setParentWindow(std::shared_ptr<QMainWindow> ptr);
        bool acceptNavigationRequest(const QUrl &, NavigationType, bool) override;

        QWebEnginePage *createWindow(WebWindowType type) override;
    private:
        std::shared_ptr<QSettings> fakesettings;
        std::shared_ptr<QMainWindow> _parent;
        std::function<void()> _parentSettings;
    };
}

#endif //QAGL_LANDING_H
