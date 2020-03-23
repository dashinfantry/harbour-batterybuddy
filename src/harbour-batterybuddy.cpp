/**
 * Battery Buddy, a Sailfish application to prolong battery lifetime
 *
 * Copyright (C) 2019-2020 Matti Viljanen
 *
 * Battery Buddy is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Battery Buddy is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with CarBudget. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Matti Viljanen
 */
#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>

#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlEngine>
#include <QTimer>
#include <QDebug>
#include <iostream>

#include "battery.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/harbour-batterybuddy.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //   - SailfishApp::pathToMainQml() to get a QUrl to the main QML file
    //
    // To display the view, call "show()" (will show fullscreen on device).

    const char* logEnvVar = "QT_LOGGING_RULES";
    for(int i = 1; i < argc; i++) {
        if(!strcmp(argv[i],"-v")) {
            std::cout << "Battery Buddy " << APP_VERSION << std::endl << std::flush;
            return 0;
        }
        else if(!strcmp(argv[i],"--verbose"))
            qputenv(logEnvVar, "*.info=true;*.debug=false");
    }
    if(!qEnvironmentVariableIsSet(logEnvVar))
        qputenv(logEnvVar, "*.info=false;*.debug=false");


    QGuiApplication* app = SailfishApp::application(argc, argv);
    QQuickView* view = SailfishApp::createView();

    qDebug() << "Application name:" << app->applicationName();
    qDebug() << "Organization name:" << app->organizationName();

    Settings* settings = new Settings();
    Battery* battery = new Battery(settings);

    QTimer* updater = new QTimer();
    QObject::connect(updater, SIGNAL(timeout()), battery, SLOT(updateData()));
    updater->start(3000);

    view->rootContext()->setContextProperty("battery", battery);
    view->rootContext()->setContextProperty("settings", settings);
    view->rootContext()->setContextProperty("app_version", APP_VERSION);

    view->setSource(SailfishApp::pathTo("qml/harbour-batterybuddy.qml"));
    view->showFullScreen();

    qInfo() << "Launching GUI...";

    int retval = app->exec();

    qInfo() << "Exiting...";

    updater->stop();
    battery->blockSignals(true);
    battery->setChargingEnabled(true);

    delete updater;
    delete battery;
    delete settings;

    qInfo() << "Goodbye!";

    return retval;
}
