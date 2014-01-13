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

#include "bookmarksmanager.h"
#include "global.h"
#include "bookmarksitem.h"

BookmarksManager * BookmarksManager::m_bookmarksManager = 0;

BookmarksManager * BookmarksManager::bookmarksManager()
{
    if(!m_bookmarksManager)
    {
        m_bookmarksManager = new BookmarksManager();
    }
    return m_bookmarksManager;
}

int BookmarksManager::count() const
{
    return m_rootFolderItem->childrenCount();
}

BookmarksItem * BookmarksManager::item(const QModelIndex & index) const
{
    return static_cast<BookmarksItem *>(index.internalPointer());
}

BookmarksItem * BookmarksManager::addItem(QString title, QUrl url, const QModelIndex & parentIndex)
{
    return insertItem(rowCount(parentIndex), title, url, parentIndex);
}

BookmarksItem * BookmarksManager::insertItem(int index, QString title, QUrl url, const QModelIndex & parentIndex)
{
    BookmarksItem * item = new BookmarksItem(title, url);
    BookmarksItem * parent = this->item(parentIndex);
    if(index < 0)
    {
        index = 0;
    }
    if(index > rowCount(parentIndex))
    {
        index = rowCount(parentIndex);
    }
    beginInsertRows(parentIndex, index, index);
    parent->insertChild(index, item);
    endInsertRows();
    return item;
}

void BookmarksManager::removeItem(const QModelIndex & index)
{
    if(index.parent().isValid())
    {
        BookmarksItem * parent = this->item(index.parent());
        beginRemoveRows(index.parent(), index.row(), index.row());
        parent->removeChild(index.row());
        endRemoveRows();
    }
}

QModelIndex BookmarksManager::index(int row, int column, const QModelIndex & parent) const
{
    BookmarksItem * item = 0;
    if(parent.isValid())
    {
        item = this->item(parent)->child(row);
    }
    else if(!row)
    {
        item = m_rootFolderItem;
    }
    if(!item || column > 1)
    {
        return QModelIndex();
    }
    else
    {
        return createIndex(row, column, item);
    }
}

QModelIndex BookmarksManager::parent(const QModelIndex & index) const
{
    if(!index.isValid())
    {
        return QModelIndex();
    }
    else
    {
        BookmarksItem * item = this->item(index);
        BookmarksItem * parent = qobject_cast<BookmarksItem *>(item->parent());
        if(parent)
        {
            int i = parent->index();
            if(i != -1)
            {
                return createIndex(i, 0, parent);
            }
            else if(parent == m_rootFolderItem)
            {
                return createIndex(0, 0, parent);
            }
        }
        return QModelIndex();
    }
}

int BookmarksManager::columnCount(const QModelIndex & /*parent*/) const
{
    return 2;
}

int BookmarksManager::rowCount(const QModelIndex & parent) const
{
    if(parent.column() > 0)
    {
        return 0;
    }
    else if(parent.isValid())
    {
        return this->item(parent)->childrenCount();
    }
    else
    {
        return 1;
    }
}

QVariant BookmarksManager::data(const QModelIndex & index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    BookmarksItem * item = this->item(index);
    if(role == Qt::DecorationRole && index.column() == 0)
    {
        QUrl url = item->url();
        if(!url.isEmpty())
        {
            return Global::notEmpty(QWebSettings::iconForUrl(url));
        }
        else if(item != m_rootFolderItem)
        {
            return QIcon::fromTheme("folder", QIcon(":/folder.png"));
        }
        else
        {
            return QIcon::fromTheme("folder-bookmark", QIcon(":/folder-bookmark.png"));
        }
    }
    else if(role == Qt::DisplayRole && index.column() == 0)
    {
        return Global::notEmpty(item->title());
    }
    else if(role == Qt::DisplayRole && index.column() == 1)
    {
        return item->url().toString();
    }
    else
    {
        return QVariant();
    }
}

QVariant BookmarksManager::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == 0)
        {
            return trUtf8("Title");
        }
        else if(section == 1)
        {
            return trUtf8("Address");
        }
        else
        {
            return QVariant();
        }
    }
    else
    {
        return QVariant();
    }
}

Qt::DropActions BookmarksManager::supportedDropActions() const
{
    return Qt::MoveAction;
}

Qt::ItemFlags BookmarksManager::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    if(!index.isValid())
    {
        return flags;
    }
    flags |= Qt::ItemIsDragEnabled | Qt::ItemIsEditable;
    BookmarksItem * item = this->item(index);
    if(item->url().isEmpty())
    {
        return flags | Qt::ItemIsDropEnabled;
    }
    return flags;
}

QStringList BookmarksManager::mimeTypes() const
{
    QStringList types;
    types += "text/plain";
    return types;
}

QMimeData * BookmarksManager::mimeData(const QModelIndexList & indexes) const
{
    QMimeData * mimeData = new QMimeData();
    QString text;
    foreach(QModelIndex index, indexes)
    {
        if(index.isValid() && !index.column())
        {
            text += QString::number(reinterpret_cast<int>(item(index))) + "\n";
        }
    }
    mimeData->setText(text);
    return mimeData;
}

bool BookmarksManager::dropMimeData(const QMimeData * data, Qt::DropAction action, int row,
                                    int /*column*/, const QModelIndex & parent)
{
    if(action == Qt::IgnoreAction)
    {
        return true;
    }
    if(!data->hasText())
    {
        return false;
    }
    QStringList itemStrings = data->text().split("\n", QString::SkipEmptyParts);
    foreach(QString itemString, itemStrings)
    {
        BookmarksItem * item = reinterpret_cast<BookmarksItem *>(itemString.toInt());
        int index;
        if(row != -1)
        {
            index = row;
            row++;
        }
        else
        {
            index = rowCount(parent);
        }
        BookmarksItem * parentItem = this->item(parent);
        beginInsertRows(parent, index, index);
        parentItem->insertChild(index, item);
        endInsertRows();
    }
    return true;
}

bool BookmarksManager::removeRows(int row, int count, const QModelIndex & parent)
{
    if(parent.isValid())
    {
        BookmarksItem * parentItem = this->item(parent);
        for(int i = 0; i < count; i++)
        {
            beginRemoveRows(parent, row, row);
            parentItem->removeChild(row, false);
            endRemoveRows();
        }
    }
    return true;
}

bool BookmarksManager::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!index.parent().isValid())
    {
        return false;
    }
    BookmarksItem * item = this->item(index);
    if(role == Qt::DisplayRole && index.column() == 0)
    {
        item->setTitle(value.toString());
        emit dataChanged(index, index);
        return true;
    }
    else if(role == Qt::DisplayRole && index.column() == 1 && !item->url().isEmpty())
    {
        item->setUrl(QUrl(value.toString()));
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

void BookmarksManager::load()
{
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QFile file(path + "/bookmarks.xml");
    if(!QDir().mkpath(path) || !file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QXmlStreamReader xml(&file);
    while(!xml.atEnd())
    {
        xml.readNext();
        if(xml.isStartElement() && xml.name() == "xbel" &&
           xml.attributes().value("version") == "1.0")
        {
            while(!xml.atEnd())
            {
                xml.readNext();
                if(xml.isEndElement())
                {
                    break;
                }
                if(xml.isStartElement())
                {
                    if(xml.name() == "folder")
                    {
                        loadFolder(xml, index(0, 0, QModelIndex()));
                    }
                    else if(xml.name() == "bookmark")
                    {
                        loadBookmark(xml, index(0, 0, QModelIndex()));
                    }
                    else
                    {
                        xml.skipCurrentElement();
                    }
                }
            }
        }
    }
    file.close();
}

void BookmarksManager::save()
{
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QFile file(path + "/bookmarks.xml");
    if(!QDir().mkpath(path) || !file.open(QIODevice::WriteOnly))
    {
        return;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("xbel");
    xml.writeAttribute("version", "1.0");
    for(int i = 0; i < count(); i++)
    {
        BookmarksItem * item = this->item(index(i, 0, index(0, 0, QModelIndex())));
        saveItem(xml, item);
    }
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();
}

BookmarksManager::BookmarksManager()
{
    m_rootFolderItem = new BookmarksItem(trUtf8("Bookmarks"), QUrl(), this);
}

void BookmarksManager::loadBookmark(QXmlStreamReader & xml, const QModelIndex & parentIndex)
{
    QUrl url(xml.attributes().value("href").toString());
    QString title;
    while(!xml.atEnd())
    {
        xml.readNext();
        if(xml.isEndElement())
        {
            break;
        }
        if(xml.isStartElement())
        {
            if(xml.name() == "title")
            {
                title = xml.readElementText();
            }
            else
            {
                xml.skipCurrentElement();
            }
        }
    }
    addItem(title, url, parentIndex);
}

void BookmarksManager::loadFolder(QXmlStreamReader & xml, const QModelIndex & parentIndex)
{
    BookmarksItem * item = addItem(QString(), QUrl(), parentIndex);
    QModelIndex newParentIndex = this->index(item->index(), 0, parentIndex);
    while(!xml.atEnd())
    {
        xml.readNext();
        if(xml.isEndElement())
        {
            break;
        }
        if(xml.isStartElement())
        {
            if(xml.name() == "title")
            {
                item->setTitle(xml.readElementText());
            }
            else if(xml.name() == "folder")
            {
                loadFolder(xml, newParentIndex);
            }
            else if(xml.name() == "bookmark")
            {
                loadBookmark(xml, newParentIndex);
            }
            else
            {
                xml.skipCurrentElement();
            }
        }
    }
}

void BookmarksManager::saveItem(QXmlStreamWriter & xml, BookmarksItem * item)
{
    if(item->url().isEmpty())
    {
        xml.writeStartElement("folder");
        xml.writeTextElement("title", item->title());
        for(int i = 0; i < item->childrenCount(); i++)
        {
            saveItem(xml, item->child(i));
        }
        xml.writeEndElement();
    }
    else
    {
        xml.writeStartElement("bookmark");
        xml.writeAttribute("href", item->url().toString());
        xml.writeTextElement("title", item->title());
        xml.writeEndElement();
    }
}
