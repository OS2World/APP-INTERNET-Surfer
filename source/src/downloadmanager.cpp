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

#include "downloadmanager.h"
#include "downloaditem.h"

DownloadManager * DownloadManager::m_downloadManager = 0;

DownloadManager * DownloadManager::downloadManager()
{
    if(!m_downloadManager)
    {
        m_downloadManager = new DownloadManager();
    }
    return m_downloadManager;
}

QString DownloadManager::sizeString(qint64 bytes)
{
    if(bytes >= 1024 * 1024 * 1024)
    {
        return QString::number((double) bytes / (1024 * 1024 * 1024), 'f', 1) + " " + trUtf8("Gb");
    }
    else if(bytes >= 1024 * 1024)
    {
        return QString::number((double) bytes / (1024 * 1024), 'f', 1) + " " + trUtf8("Mb");
    }
    else if(bytes >= 1024)
    {
        return QString::number((double) bytes / 1024, 'f', 1) + " " + trUtf8("Kb");
    }
    else
    {
        return QString::number(bytes) + " " + trUtf8("b");
    }
}

DownloadItem * DownloadManager::item(const QModelIndex & index) const
{
    return m_items.value(index.row());
}

int DownloadManager::count() const
{
    return m_items.count();
}

DownloadItem * DownloadManager::download(const QUrl & from, const QUrl & to)
{
    DownloadItem * item = new DownloadItem(from, to, this);
    connect(item, SIGNAL(started()), this, SLOT(updateData()));
    connect(item, SIGNAL(progressChanged(int)), this, SLOT(updateData()));
    connect(item, SIGNAL(finished()), this, SLOT(updateData()));
    beginInsertRows(QModelIndex(), 0, 0);
    m_items.prepend(item);
    endInsertRows();
    emit itemsChanged();
    item->start();
    return item;
}

void DownloadManager::removeItem(const QModelIndex & index)
{
    DownloadItem * item = this->item(index);
    if(item)
    {
        item->stop();
        beginRemoveRows(QModelIndex(), index.row(), index.row());
        item->deleteLater();
        m_items.removeAt(index.row());
        endRemoveRows();
        emit itemsChanged();
    }
}

int DownloadManager::columnCount(const QModelIndex & /*parent*/) const
{
    return 3;
}

int DownloadManager::rowCount(const QModelIndex & /*parent*/) const
{
    return count();
}

QVariant DownloadManager::data(const QModelIndex & index, int role) const
{
    DownloadItem * item = this->item(index);
    if(!index.isValid() || index.column() >= 3 || index.row() >= count())
    {
        return QVariant();
    }
    else if(role == Qt::DisplayRole && index.column() == 0)
    {
        return item->from().toString();
    }
    else if(role == Qt::DisplayRole && index.column() == 1)
    {
        return item->to().toString();
    }
    else if(role == Qt::UserRole && index.column() == 2)
    {
        return item->progress();
    }
    else if(role == Qt::DisplayRole && index.column() == 2)
    {
        QString text = sizeString(item->received()) + " / " + sizeString(item->total()) + " (";
        if(item->isFinished())
        {
            if(item->progress() == 100)
            {
                text += trUtf8("Finished");
            }
            else
            {
                text += trUtf8("Aborted");
            }
        }
        else
        {
            text += QString::number(item->progress()) + "%";
        }
        text += ")";
        return text;
    }
    else
    {
        return QVariant();
    }
}

QVariant DownloadManager::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == 0)
        {
            return trUtf8("Source address");
        }
        else if(section == 1)
        {
            return trUtf8("Destination address");
        }
        else if(section == 2)
        {
            return trUtf8("Progress");
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

QString DownloadManager::standardPath() const
{
    return m_standardPath;
}

void DownloadManager::setStandardPath(const QString & path)
{
    m_standardPath = path;
}

void DownloadManager::load()
{
    QSettings settings;
    setStandardPath(settings.value("DownloadManager/standardPath", "").toString());
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QFile file(path + "/downloads.xml");
    if(!QDir().mkpath(path) || !file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QXmlStreamReader xml(&file);
    while(!xml.atEnd())
    {
        xml.readNext();
        if(xml.isStartElement() && xml.name() == "surfer")
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
                    if(xml.name() == "item")
                    {
                        QUrl from, to;
                        while(!xml.atEnd())
                        {
                            xml.readNext();
                            if(xml.isEndElement())
                            {
                                break;
                            }
                            if(xml.isStartElement())
                            {
                                if(xml.name() == "from")
                                {
                                    from = xml.readElementText();
                                }
                                else if(xml.name() == "to")
                                {
                                    to = xml.readElementText();
                                }
                                else
                                {
                                    xml.skipCurrentElement();
                                }
                            }
                        }
                        download(from, to);
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

void DownloadManager::save()
{
    QSettings settings;
    settings.setValue("DownloadManager/standardPath", standardPath());
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QFile file(path + "/downloads.xml");
    if(!QDir().mkpath(path) || !file.open(QIODevice::WriteOnly))
    {
        return;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("surfer");
    for(int i = count() - 1; i >= 0; i--)
    {
        DownloadItem * item = m_items.at(i);
        if(!item->isFinished())
        {
            xml.writeStartElement("item");
            xml.writeTextElement("from", item->from().toString());
            xml.writeTextElement("to", item->to().toString());
            xml.writeEndElement();
        }
    }
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();
}

DownloadManager::DownloadManager()
{
}

void DownloadManager::updateData()
{
    DownloadItem * item = qobject_cast<DownloadItem *>(sender());
    QModelIndex index = this->index(m_items.indexOf(item), 2, QModelIndex());
    emit dataChanged(index, index);
}
