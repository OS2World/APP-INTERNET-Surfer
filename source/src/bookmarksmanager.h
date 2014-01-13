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

#ifndef BOOKMARKSMANAGER_H
#define BOOKMARKSMANAGER_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

class BookmarksItem;

class BookmarksManager : public QAbstractItemModel
{
    Q_OBJECT

public:
    static BookmarksManager * bookmarksManager();

    int count() const;
    BookmarksItem * item(const QModelIndex & index) const;
    BookmarksItem * addItem(QString title, QUrl url, const QModelIndex & parentIndex);
    BookmarksItem * insertItem(int index, QString title, QUrl url, const QModelIndex & parentIndex);
    void removeItem(const QModelIndex & index);

    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex & index) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::DropActions supportedDropActions() const;
    Qt::ItemFlags flags(const QModelIndex & index) const;
    QStringList mimeTypes() const;
    QMimeData * mimeData(const QModelIndexList & indexes) const;
    bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column,
                      const QModelIndex & parent);
    bool removeRows(int row, int count, const QModelIndex & parent);
    bool setData(const QModelIndex & index, const QVariant & value, int role);

public slots:
    void load();
    void save();

protected:
    BookmarksManager();

private:
    static BookmarksManager * m_bookmarksManager;

    BookmarksItem * m_rootFolderItem;

    void loadBookmark(QXmlStreamReader & xml, const QModelIndex & parentIndex);
    void loadFolder(QXmlStreamReader & xml, const QModelIndex & parentIndex);
    void saveItem(QXmlStreamWriter & xml, BookmarksItem * item);
};

#endif // BOOKMARKSMANAGER_H
