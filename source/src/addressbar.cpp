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

#include "addressbar.h"
#include "historymanager.h"

AddressBar::AddressBar(QWebView * webView, QWidget * parent)
        : QLineEdit(parent)
        , m_webView(webView)
{
    m_focusShortcut = new QShortcut(Qt::Key_F6, this);

    m_completer = new QCompleter(this);
    m_completer->setModel(HistoryManager::historyManager());
    m_popup = new QTreeView(this);
    m_popup->header()->setResizeMode(QHeaderView::Stretch);
    m_popup->setRootIsDecorated(false);
    m_completer->setPopup(m_popup);
    setCompleter(m_completer);

    connect(this, SIGNAL(returnPressed()), this, SLOT(load()));
    connect(m_webView, SIGNAL(urlChanged(const QUrl &)), this, SLOT(setUrl(const QUrl &)));
    connect(m_focusShortcut, SIGNAL(activated()), this, SLOT(setFocus()));
    connect(m_focusShortcut, SIGNAL(activated()), this, SLOT(selectAll()));
}

QUrl AddressBar::url() const
{
    return QUrl(text());
}

void AddressBar::setUrl(const QUrl & url)
{
    setText(url.toString());
}

void AddressBar::load()
{
    QString text(this->text());
    if(QUrl(text).scheme().isEmpty())
    {
        if(text.contains('.'))
        {
            text = "http://" + text;
        }
        else
        {
            text = "http://www.google.com/search?q=" + text;
        }
    }
    QUrl url(text);
    setUrl(url);
    m_webView->load(url);
    m_webView->setFocus();
}
