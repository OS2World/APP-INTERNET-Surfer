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

#include "forwardbutton.h"
#include "global.h"
#include "menu.h"

ForwardButton::ForwardButton(QWebView * webView, QWidget * parent)
        : QToolButton(parent)
        , m_webView(webView)
{
    m_defaultAction = new QAction(this);
    m_defaultAction->setData(1);
    setDefaultAction(m_defaultAction);

    setAutoRaise(true);
    setIcon(QIcon::fromTheme("go-next", QIcon(":/go-next.png")));
    setToolTip(trUtf8("Go forward"));
    setShortcut(QKeySequence::Forward);
    setPopupMode(QToolButton::MenuButtonPopup);

    m_popupMenu = new Menu(this);
    setMenu(m_popupMenu);

    updateButton();

    connect(m_webView, SIGNAL(urlChanged(const QUrl &)), this, SLOT(updateButton()));
    connect(m_defaultAction, SIGNAL(triggered()), this, SLOT(goForward()));
    connect(m_popupMenu, SIGNAL(aboutToShow()), this, SLOT(updatePopupMenu()));
}

void ForwardButton::updateButton()
{
    setEnabled(m_webView->history()->canGoForward());
}

void ForwardButton::goForward()
{
    QAction * action = qobject_cast<QAction *>(sender());
    int index = action->data().toInt();
    QWebHistory * history = m_webView->history();
    history->goToItem(history->forwardItems(index).last());
}

void ForwardButton::updatePopupMenu()
{
    m_popupMenu->clear();
    QList<QWebHistoryItem> items = m_webView->history()->forwardItems(10);
    for(int i = 0; i < items.size() ; i++)
    {
        QWebHistoryItem item = items.at(i);
        QIcon icon = Global::notEmpty(QWebSettings::iconForUrl(item.url()));
        QString title = Global::notEmpty(item.title());
        QAction * action = m_popupMenu->addAction(icon, Global::notLong(title));
        action->setToolTip(title + "\n" + item.url().toString());
        action->setData(i + 1);
        connect(action, SIGNAL(triggered()), this, SLOT(goForward()));
    }
}
