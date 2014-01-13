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

#include "mainwindow.h"
#include "tabmanager.h"
#include "historymanager.h"
#include "bookmarksmanager.h"
#include "settingsmanager.h"
#include "downloadmanager.h"
#include "networkaccessmanager.h"

MainWindow::MainWindow()
{
    setWindowTitle("Surfer");

    QSettings settings;
    QRect geometry = settings.value("MainWindow/geometry").toRect();
    if(!geometry.isEmpty())
    {
        setGeometry(geometry);
    }
    else
    {
        resize(900, 600);
    }

    m_exitShortcut = new QShortcut(Qt::CTRL + Qt::Key_Q, this);
    connect(m_exitShortcut, SIGNAL(activated()), this, SLOT(close()));

    setCentralWidget(TabManager::tabManager());

    SettingsManager::settingsManager()->load();
    NetworkAccessManager::networkAccessManager()->load();
    TabManager::tabManager()->load();
    HistoryManager::historyManager()->load();
    BookmarksManager::bookmarksManager()->load();
    DownloadManager::downloadManager()->load();
}

MainWindow::~MainWindow()
{
    DownloadManager::downloadManager()->deleteLater();
    BookmarksManager::bookmarksManager()->deleteLater();
    HistoryManager::historyManager()->deleteLater();
    TabManager::tabManager()->deleteLater();
    NetworkAccessManager::networkAccessManager()->deleteLater();
    SettingsManager::settingsManager()->deleteLater();
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    QSettings settings;
    settings.setValue("MainWindow/geometry", geometry());

    DownloadManager::downloadManager()->save();
    BookmarksManager::bookmarksManager()->save();
    HistoryManager::historyManager()->save();
    TabManager::tabManager()->save();
    NetworkAccessManager::networkAccessManager()->save();
    SettingsManager::settingsManager()->save();

    event->accept();
}
