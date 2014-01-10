/*
 * Copyright 2013 Canonical Ltd.
 *
 * This file is part of webbrowser-app.
 *
 * webbrowser-app is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * webbrowser-app is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"
#include "settings.h"
#include "webbrowser-app.h"

// Qt
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QMetaObject>
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtQuick/QQuickWindow>

// System
#include <unistd.h>
#include <stdio.h>

WebbrowserApp::WebbrowserApp(int& argc, char** argv)
    : BrowserApplication(argc, argv)
{
}

bool WebbrowserApp::initialize()
{
    // Re-direct webapps to the dedicated container for backward compatibility
    // with 13.10
    Q_FOREACH(const QString& argument, m_arguments) {
        if (argument.startsWith("--webapp")) {
            qWarning() << "Deprecated webapp options: use the webapp-container program instead" << endl;

            int size = m_arguments.size();
            char *argv[size + 2];
            argv[0] = (char *)"webapp-container";
            int i = 0;
            foreach(QString s, m_arguments) {
                argv[i+1] = new char[s.toLocal8Bit().size()+1];
                strcpy(argv[i+1], s.toLocal8Bit().constData());
                i++;
            }
            argv[i+1] = (char) NULL;

            exit(execvp(argv[0], argv));
        }
    }

    if (BrowserApplication::initialize("webbrowser/webbrowser-app.qml")) {
        m_window->setProperty("chromeless", m_arguments.contains("--chromeless"));
        QList<QUrl> urls = this->urls();
        if (urls.isEmpty()) {
            Settings settings;
            urls.append(settings.homepage());
        }
        QObject* browser = (QObject*) m_window;
        Q_FOREACH(const QUrl& url, urls) {
            QMetaObject::invokeMethod(browser, "newTab", Q_ARG(QVariant, url), Q_ARG(QVariant, true));
        }
        return true;
    } else {
        return false;
    }
}

void WebbrowserApp::printUsage() const
{
    QTextStream out(stdout);
    QString command = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    out << "Usage: " << command << " [-h|--help] [--chromeless] [--fullscreen] [--maximized] [--inspector] [--app-id=APP_ID] [URL]" << endl;
    out << "Options:" << endl;
    out << "  -h, --help         display this help message and exit" << endl;
    out << "  --chromeless       do not display any chrome" << endl;
    out << "  --fullscreen       display full screen" << endl;
    out << "  --maximized        opens the application maximized" << endl;
    out << "  --inspector        run a remote inspector on port " << REMOTE_INSPECTOR_PORT << endl;
    out << "  --app-id=APP_ID    run the application with a specific APP_ID" << endl;
}

int main(int argc, char** argv)
{
    WebbrowserApp app(argc, argv);
    if (app.initialize()) {
        return app.run();
    } else {
        return 0;
    }
}
