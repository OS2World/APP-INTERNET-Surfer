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

#include "restorebutton.h"
#include "global.h"
#include "tabmanager.h"
#include "restoreitem.h"
#include "menu.h"

RestoreButton::RestoreButton(QWidget * parent)
        : QToolButton(parent)
{
    m_defaultAction = new QAction(this);
    m_defaultAction->setData(0);
    setDefaultAction(m_defaultAction);

    setAutoRaise(true);
    setIcon(QIcon::fromTheme("user-trash", QIcon(":/user-trash.png")));
    setToolTip(trUtf8("Restore last closed tab"));
    setShortcut(Qt::CTRL + Qt::Key_R);
    setPopupMode(QToolButton::MenuButtonPopup);

    m_popupMenu = new Menu(this);
    setMenu(m_popupMenu);

    updateButton();

    connect(TabManager::tabManager(), SIGNAL(restoreListChanged()), this, SLOT(updateButton()));
    connect(m_defaultAction, SIGNAL(triggered()), this, SLOT(restore()));
    connect(m_popupMenu, SIGNAL(aboutToShow()), this, SLOT(updatePopupMenu()));
}

void RestoreButton::updateButton()
{
    setEnabled(!TabManager::tabManager()->restoreItems(1).isEmpty());
}

void RestoreButton::restore()
{
    QAction * action = qobject_cast<QAction *>(sender());
    int index = action->data().toInt();
    TabManager::tabManager()->restoreTab(index);
}

void RestoreButton::updatePopupMenu()
{
    m_popupMenu->clear();
    QList<RestoreItem *> items = TabManager::tabManager()->restoreItems(10);
    for(int i = 0; i < items.size(); i++)
    {
        RestoreItem * item = items.at(i);
        QIcon icon = Global::notEmpty(QWebSettings::iconForUrl(item->url()));
        QString title = Global::notEmpty(item->title());
        QAction * action = m_popupMenu->addAction(icon, Global::notLong(title));
        action->setToolTip(title + "\n" + item->url().toString());
        action->setData(i);
        connect(action, SIGNAL(triggered()), this, SLOT(restore()));
    }
    m_popupMenu->addSeparator();
    QAction * action = m_popupMenu->addAction(QIcon::fromTheme("edit-clear-list", QIcon(":/edit-clear-list.png")), trUtf8("Clear closed tab list"));
    connect(action, SIGNAL(triggered()), TabManager::tabManager(), SLOT(clearRestoreList()));
}
