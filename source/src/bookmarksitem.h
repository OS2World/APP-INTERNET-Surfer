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

#ifndef BOOKMARKSITEM_H
#define BOOKMARKSITEM_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

class BookmarksItem : public QObject
{
    Q_OBJECT

public:
    BookmarksItem(const QString & title, const QUrl & url = QUrl(), QObject * parent = 0);
    QString title() const;
    void setTitle(const QString & title);
    QUrl url() const;
    void setUrl(const QUrl & url);
    int childrenCount();
    BookmarksItem * child(int index);
    int index();
    void addChild(BookmarksItem * child);
    void insertChild(int index, BookmarksItem * child);
    void removeChild(int index, bool d = true);

private:
    QString m_title;
    QUrl m_url;
    QList<BookmarksItem *> m_children;
};

#endif // BOOKMARKSITEM_H
