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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

class MainWindow;

class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int & argc, char ** argv);
    ~Application();

private:
    QTranslator * m_translator0;
    QTranslator * m_translator1;
    QTranslator * m_translator2;
    QTcpSocket * m_initialSocket;
    QTcpServer * m_urlsServer;
    QTcpSocket * m_urlsSocket;
    MainWindow * m_window;
    QList<QUrl> m_clUrls;

private slots:
    void writeUrls();
    void acceptConnection();
    void openUrls();
};

#endif // APPLICATION_H
