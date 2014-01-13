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

#include "historybutton.h"
#include "global.h"
#include "historytab.h"
#include "historymanager.h"
#include "historyitem.h"
#include "tabmanager.h"
#include "menu.h"

int HistoryButton::m_count = 0;
HistoryTab * HistoryButton::m_tab  = 0;

HistoryButton::HistoryButton(QWidget * parent)
        : QToolButton(parent)
{
    m_count++;
    if(m_count == 1)
    {
        m_tab = new HistoryTab();
    }

    setAutoRaise(true);
    setIcon(QIcon::fromTheme("view-history", QIcon(":/view-history.png")));
    setToolTip(trUtf8("Last visited pages"));
    setShortcut(Qt::CTRL + Qt::Key_H);
    setPopupMode(QToolButton::InstantPopup);

    m_popupMenu = new Menu(this);
    setMenu(m_popupMenu);
    m_openTabAction = new QAction(QIcon::fromTheme("view-history", QIcon(":/view-history.png")), trUtf8("History"), this);

    connect(m_popupMenu, SIGNAL(aboutToShow()), this, SLOT(updatePopupMenu()));
    connect(m_openTabAction, SIGNAL(triggered()), this, SLOT(openTab()));
}

HistoryButton::~HistoryButton()
{
    m_count--;
    if(!m_count)
    {
        m_tab->deleteLater();
    }
}

void HistoryButton::openTab()
{
    TabManager::tabManager()->addTab(m_tab);
}

void HistoryButton::openUrl()
{
    QAction * action = qobject_cast<QAction *>(sender());
    TabManager::tabManager()->addTab(action->data().toUrl());
}

void HistoryButton::updatePopupMenu()
{
    m_popupMenu->clear();
    m_popupMenu->addAction(m_openTabAction);
    m_popupMenu->addSeparator();
    HistoryManager * manager = HistoryManager::historyManager();
    for(int i = 0; i < 10 && i < manager->count(); i++)
    {
        HistoryItem * item = manager->item(manager->index(i));
        QIcon icon = Global::notEmpty(QWebSettings::iconForUrl(item->url()));
        QString title = Global::notEmpty(item->title());
        QAction * action = m_popupMenu->addAction(icon, Global::notLong(title));
        action->setToolTip(title + "\n" + item->url().toString());
        action->setData(item->url());
        connect(action, SIGNAL(triggered()), this, SLOT(openUrl()));
    }
    if(!manager->count())
    {
        QAction * action = m_popupMenu->addAction(QString("(") + trUtf8("Empty") + ")");
        action->setEnabled(false);
    }
}
