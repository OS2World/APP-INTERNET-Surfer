/*******************************************************************************
*    Surfer web browser                                                        *
*    Copyright (C) 2010  Andrianov Ivan                                        *
*                                                                              *
*    This program is free software: you can redistribute it and/or modify      *
*    it under the terms of the GNU General Public License as published by      *
*    the Free Software Foundation, either version 3 of the License, or         *
*    (at your option) any later version.                                       *
*                                                                              *
*    This program is distributed in the hope that it will be useful,           *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
*    GNU General Public License for more details.                              *
*                                                                              *
*    You should have received a copy of the GNU General Public License         *
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
*******************************************************************************/

#include "application.h"
#include "mainwindow.h"
#include "tabmanager.h"

Application::Application(int & argc, char ** argv)
        : QApplication(argc, argv)
        , m_urlsSocket(0)
        , m_window(0)
{
    setOrganizationName("surfer");
    setApplicationName("surfer");
    setApplicationVersion("1.0");
    setWindowIcon(QIcon::fromTheme("applications-internet", QIcon(":/applications-internet.png")));

    m_translator0 = new QTranslator(this);
    m_translator0->load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    installTranslator(m_translator0);

    m_translator1 = new QTranslator(this);
    m_translator1->load(QLocale::system().name(), QApplication::applicationDirPath() + "/translations");
    installTranslator(m_translator1);

#ifdef Q_WS_X11
    m_translator2 = new QTranslator(this);
    m_translator2->load(QLocale::system().name(), "/usr/share/surfer/translations");
    installTranslator(m_translator2);
#endif

    for(int i = 1; i < argc; i++)
    {
        QUrl url(argv[i]);
        if(!url.scheme().isEmpty())
        {
            m_clUrls += url;
        }
    }

    m_urlsServer = new QTcpServer(this);
    bool isFirstRun = m_urlsServer->listen(QHostAddress::LocalHost, 7777);

    if(isFirstRun)
    {
        connect(m_urlsServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
        m_window = new MainWindow();
        m_window->show();
    }

    m_initialSocket = new QTcpSocket(this);
    m_initialSocket->connectToHost(QHostAddress::LocalHost, 7777);
    connect(m_initialSocket, SIGNAL(connected()), this, SLOT(writeUrls()));
    if(!isFirstRun)
    {
        connect(m_initialSocket, SIGNAL(disconnected()), this, SLOT(quit()));
    }
}

Application::~Application()
{
    if(m_window)
    {
        m_window->deleteLater();
    }
}

void Application::writeUrls()
{
    QByteArray data;
    foreach(QUrl url, m_clUrls)
    {
        data += url.toString().toUtf8();
        data += "\n";
    }
    m_initialSocket->write(data);
    m_initialSocket->disconnectFromHost();
}

void Application::acceptConnection()
{
    if(m_urlsSocket)
    {
        m_urlsSocket->deleteLater();
    }
    m_urlsSocket = m_urlsServer->nextPendingConnection();
    connect(m_urlsSocket, SIGNAL(readyRead()), this, SLOT(openUrls()));
}

void Application::openUrls()
{
    QByteArray array;
    while(!(array = m_urlsSocket->readLine()).isEmpty())
    {
        QString urlString = QString::fromUtf8(array.data()).trimmed();
        TabManager::tabManager()->addTab(QUrl(urlString));
    }
}
