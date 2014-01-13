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

#ifndef WEBTAB_H
#define WEBTAB_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>
#include "itab.h"

class WebView;
class BackButton;
class ForwardButton;
class ReloadStopButton;
class AddressBar;
class RestoreButton;
class HistoryButton;
class BookmarksButton;
class DownloadButton;
class SettingsButton;
class SearchBar;
class ZoomBar;

class WebTab : public ITab
{
    Q_OBJECT

public:
    WebTab(const QUrl & initialUrl, QWidget * parent = 0);

    WebView * webView() const;
    QUrl url() const;

    QString title() const;
    QIcon icon() const;

private:
    QUrl m_initialUrl;

    QVBoxLayout * m_layout;

    WebView * m_webView;

    QToolBar * m_toolBar;
    BackButton * m_backButton;
    ForwardButton * m_forwardButton;
    ReloadStopButton * m_reloadStopButton;
    AddressBar * m_addressBar;
    RestoreButton * m_restoreButton;
    HistoryButton * m_historyButton;
    BookmarksButton * m_bookmarksButton;
    DownloadButton * m_downloadButton;
    SettingsButton * m_settingsButton;

    QStatusBar * m_statusBar;
    QProgressBar * m_progressBar;
    SearchBar * m_searchBar;
    ZoomBar * m_zoomBar;

    void setupWebView();
    void setupToolBar();
    void setupStatusBar();
};

#endif // WEBTAB_H
