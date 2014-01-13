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

#include "webview.h"
#include "tabmanager.h"
#include "webtab.h"
#include "historymanager.h"
#include "webpage.h"

WebView::WebView(QWidget * parent)
        : QWebView(parent)
{
    m_webPage = new WebPage(this);
    setPage(m_webPage);

    m_copyShortcut = new QShortcut(QKeySequence::Copy, this);

    connect(this, SIGNAL(titleChanged(const QString &)), this, SLOT(updateHistory()));
    connect(m_copyShortcut, SIGNAL(activated()), this, SLOT(copySelectedText()));
}

QWebView * WebView::createWindow(QWebPage::WebWindowType /*type*/)
{
    return TabManager::tabManager()->addChildTab()->webView();
}

void WebView::updateHistory()
{
    if(!title().isEmpty())
    {
        HistoryManager::historyManager()->addItem(url(), title());
    }
}

void WebView::copySelectedText()
{
    triggerPageAction(QWebPage::Copy);
}
