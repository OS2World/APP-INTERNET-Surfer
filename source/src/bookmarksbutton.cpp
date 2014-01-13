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

#include "bookmarksbutton.h"
#include "global.h"
#include "bookmarkstab.h"
#include "bookmarksfolderdialog.h"
#include "bookmarksmanager.h"
#include "bookmarksitem.h"
#include "tabmanager.h"
#include "menu.h"

int BookmarksButton::m_count = 0;
BookmarksTab * BookmarksButton::m_tab = 0;

BookmarksButton::BookmarksButton(QWebView * webView, QWidget * parent)
        : QToolButton(parent)
        , m_webView(webView)
{
    m_count++;
    if(m_count == 1)
    {
        m_tab = new BookmarksTab();
    }

    setAutoRaise(true);
    setIcon(QIcon::fromTheme("bookmarks", QIcon(":/bookmarks.png")));
    setToolTip(trUtf8("Bookmarks"));
    setShortcut(Qt::CTRL + Qt::Key_B);
    setPopupMode(QToolButton::InstantPopup);

    m_popupMenu = new Menu(this);
    setMenu(m_popupMenu);
    if(m_webView)
    {
        m_createBookmarkAction = new QAction(QIcon::fromTheme("bookmark-new", QIcon(":/bookmark-new.png")), trUtf8("Add bookmark"), this);
        connect(m_createBookmarkAction, SIGNAL(triggered()), this, SLOT(createBookmark()));
        if(m_webView->url().isEmpty())
        {
            m_createBookmarkAction->setEnabled(false);
            connect(m_webView, SIGNAL(urlChanged(const QUrl &)), this, SLOT(updateAction()));
        }
    }
    m_openTabAction = new QAction(QIcon::fromTheme("bookmarks-organize", QIcon(":/bookmarks-organize.png")), trUtf8("Manage bookmarks"), this);

    connect(m_popupMenu, SIGNAL(aboutToShow()), this, SLOT(updatePopupMenu()));
    connect(m_openTabAction, SIGNAL(triggered()), this, SLOT(openTab()));
}

BookmarksButton::~BookmarksButton()
{
    m_count--;
    if(!m_count)
    {
        m_tab->deleteLater();
    }
}

void BookmarksButton::updatePopupMenuPart(Menu * menu, BookmarksItem * item)
{
    if(menu == m_popupMenu)
    {
        if(m_webView)
        {
            menu->addAction(m_createBookmarkAction);
        }
        menu->addAction(m_openTabAction);
        menu->addSeparator();
    }
    for(int i = 0; i < item->childrenCount(); i++)
    {
        BookmarksItem * newItem = item->child(i);
        if(!newItem->url().isEmpty())
        {
            QIcon icon = Global::notEmpty(QWebSettings::iconForUrl(newItem->url()));
            QString title = Global::notEmpty(newItem->title());
            QAction * action = menu->addAction(icon, Global::notLong(title));
            action->setToolTip(title + "\n" + newItem->url().toString());
            action->setData(newItem->url());
            connect(action, SIGNAL(triggered()), this, SLOT(openUrl()));
        }
        else
        {
            Menu * newMenu = menu->addMenu(QIcon::fromTheme("folder", QIcon(":/folder.png")), newItem->title());
            updatePopupMenuPart(newMenu, newItem);
        }
    }
    if(!item->childrenCount())
    {
        QAction * action = menu->addAction(QString("(") + trUtf8("Empty") + ")");
        action->setEnabled(false);
    }
}

void BookmarksButton::createBookmark()
{
    BookmarksFolderDialog dialog(m_webView->title(), this);
    if(dialog.exec() == QDialog::Accepted && !dialog.title().isEmpty())
    {
        BookmarksManager * manager = BookmarksManager::bookmarksManager();
        manager->addItem(dialog.title(), m_webView->url(), dialog.index());
    }
}

void BookmarksButton::openTab()
{
    TabManager::tabManager()->addTab(m_tab);
}

void BookmarksButton::openUrl()
{
    QAction * action = qobject_cast<QAction *>(sender());
    TabManager::tabManager()->addTab(action->data().toUrl());
}

void BookmarksButton::updatePopupMenu()
{
    BookmarksManager * manager = BookmarksManager::bookmarksManager();
    m_popupMenu->clear();
    updatePopupMenuPart(m_popupMenu, manager->item(manager->index(0, 0, QModelIndex())));
}

void BookmarksButton::updateAction()
{
    m_createBookmarkAction->setEnabled(true);
    disconnect(m_webView, SIGNAL(urlChanged(const QUrl &)), this, SLOT(updateAction()));
}
