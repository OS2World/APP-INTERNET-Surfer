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

#include "webtab.h"
#include "webview.h"
#include "backbutton.h"
#include "forwardbutton.h"
#include "reloadstopbutton.h"
#include "addressbar.h"
#include "restorebutton.h"
#include "historybutton.h"
#include "bookmarksbutton.h"
#include "downloadbutton.h"
#include "settingsbutton.h"
#include "searchbar.h"
#include "zoombar.h"

WebTab::WebTab(const QUrl & initialUrl, QWidget * parent)
        : ITab(parent)
        , m_initialUrl(initialUrl)
{
    m_layout = new QVBoxLayout(this);
    m_layout->setMargin(0);
    m_layout->setSpacing(0);

    setupWebView();
    setupToolBar();
    setupStatusBar();

    m_addressBar->setUrl(m_initialUrl);
    m_webView->load(m_initialUrl);
}

WebView * WebTab::webView() const
{
    return m_webView;
}

QUrl WebTab::url() const
{
    QUrl url(m_webView->url());
    if(url.isEmpty())
    {
        return m_initialUrl;
    }
    else
    {
        return url;
    }
}

QString WebTab::title() const
{
    return m_webView->title();
}

QIcon WebTab::icon() const
{
    return QWebSettings::iconForUrl(url());
}

void WebTab::setupWebView()
{
    m_webView = new WebView(this);
    m_layout->addWidget(m_webView, 1);

    connect(m_webView, SIGNAL(titleChanged(const QString &)), this, SIGNAL(titleChanged(const QString &)));
    connect(m_webView, SIGNAL(iconChanged()), this, SIGNAL(iconChanged()));
}

void WebTab::setupToolBar()
{
    m_toolBar = new QToolBar(this);
    m_backButton = new BackButton(m_webView);
    m_toolBar->addWidget(m_backButton);
    m_forwardButton = new ForwardButton(m_webView);
    m_toolBar->addWidget(m_forwardButton);
    m_reloadStopButton = new ReloadStopButton(m_webView);
    m_toolBar->addWidget(m_reloadStopButton);
    m_addressBar = new AddressBar(m_webView);
    m_toolBar->addWidget(m_addressBar);
    m_restoreButton = new RestoreButton();
    m_toolBar->addWidget(m_restoreButton);
    m_historyButton = new HistoryButton();
    m_toolBar->addWidget(m_historyButton);
    m_bookmarksButton = new BookmarksButton(m_webView);
    m_toolBar->addWidget(m_bookmarksButton);
    m_downloadButton = new DownloadButton();
    m_toolBar->addWidget(m_downloadButton);
    m_settingsButton = new SettingsButton();
    m_toolBar->addWidget(m_settingsButton);
    m_layout->insertWidget(0, m_toolBar);
}

void WebTab::setupStatusBar()
{
    m_statusBar = new QStatusBar(this);
    m_progressBar = new QProgressBar();
    m_progressBar->setFixedHeight(16);
    m_progressBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    m_progressBar->setRange(0, 100);
    m_statusBar->addPermanentWidget(m_progressBar);
    m_searchBar = new SearchBar(m_webView, this);
    m_statusBar->addPermanentWidget(m_searchBar);
    m_zoomBar = new ZoomBar(m_webView, this);
    m_statusBar->addPermanentWidget(m_zoomBar);
    m_layout->addWidget(m_statusBar);

    connect(m_webView, SIGNAL(loadStarted()), m_progressBar, SLOT(show()));
    connect(m_webView, SIGNAL(loadProgress(int)), m_progressBar, SLOT(show()));
    connect(m_webView, SIGNAL(loadProgress(int)), m_progressBar, SLOT(setValue(int)));
    connect(m_webView, SIGNAL(loadFinished(bool)), m_progressBar, SLOT(hide()));
    connect(m_webView->page(), SIGNAL(linkHovered(const QString &,const QString &,const QString &)),
            m_statusBar, SLOT(showMessage(const QString &)));
}
