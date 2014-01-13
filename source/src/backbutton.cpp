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

#include "backbutton.h"
#include "global.h"
#include "menu.h"

BackButton::BackButton(QWebView * webView, QWidget * parent)
        : QToolButton(parent)
        , m_webView(webView)
{
    m_defaultAction = new QAction(this);
    m_defaultAction->setData(-1);
    setDefaultAction(m_defaultAction);

    setAutoRaise(true);
    setIcon(QIcon::fromTheme("go-previous", QIcon(":/go-previous.png")));
    setToolTip(trUtf8("Go back"));
    setShortcut(QKeySequence::Back);
    setPopupMode(QToolButton::MenuButtonPopup);

    m_popupMenu = new Menu(this);
    setMenu(m_popupMenu);

    updateButton();

    connect(m_webView, SIGNAL(urlChanged(const QUrl &)), this, SLOT(updateButton()));
    connect(m_defaultAction, SIGNAL(triggered()), this, SLOT(goBack()));
    connect(m_popupMenu, SIGNAL(aboutToShow()), this, SLOT(updatePopupMenu()));
}

void BackButton::updateButton()
{
    setEnabled(m_webView->history()->canGoBack());
}

void BackButton::goBack()
{
    QAction * action = qobject_cast<QAction *>(sender());
    int index = action->data().toInt();
    QWebHistory * history = m_webView->history();
    history->goToItem(history->backItems(-index).first());
}

void BackButton::updatePopupMenu()
{
    m_popupMenu->clear();
    QList<QWebHistoryItem> items = m_webView->history()->backItems(10);
    for(int i = items.size() - 1; i >= 0; i--)
    {
        QWebHistoryItem item = items.at(i);
        QIcon icon = Global::notEmpty(QWebSettings::iconForUrl(item.url()));
        QString title = Global::notEmpty(item.title());
        QAction * action = m_popupMenu->addAction(icon, Global::notLong(title));
        action->setToolTip(title + "\n" + item.url().toString());
        action->setData(i - items.size());
        connect(action, SIGNAL(triggered()), this, SLOT(goBack()));
    }
}
