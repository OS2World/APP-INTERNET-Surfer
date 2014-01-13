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

#include "bookmarksitem.h"

BookmarksItem::BookmarksItem(const QString & title, const QUrl & url, QObject * parent)
        : QObject(parent)
        , m_title(title)
        , m_url(url)
{
}

QString BookmarksItem::title() const
{
    return m_title;
}

void BookmarksItem::setTitle(const QString & title)
{
    m_title = title;
}

QUrl BookmarksItem::url() const
{
    return m_url;
}

void BookmarksItem::setUrl(const QUrl & url)
{
    m_url = url;
}

int BookmarksItem::childrenCount()
{
    return m_children.count();
}

BookmarksItem * BookmarksItem::child(int index)
{
    return m_children.value(index);
}

int BookmarksItem::index()
{
    BookmarksItem * parent = qobject_cast<BookmarksItem *>(this->parent());
    if(parent)
    {
        for(int i = 0; i < parent->childrenCount(); i++)
        {
            if(parent->child(i) == this)
            {
                return i;
            }
        }
    }
    return -1;
}

void BookmarksItem::addChild(BookmarksItem * child)
{
    insertChild(childrenCount(), child);
}

void BookmarksItem::insertChild(int index, BookmarksItem * child)
{
    child->setParent(this);
    m_children.insert(index, child);
}

void BookmarksItem::removeChild(int index, bool d)
{
    BookmarksItem * child = this->child(index);
    if(child)
    {
        if(d)
        {
            child->deleteLater();
        }
        m_children.removeAt(index);
    }
}
