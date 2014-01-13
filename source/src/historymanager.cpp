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

#include "historymanager.h"
#include "global.h"
#include "historyitem.h"
#include "historyinterface.h"

HistoryManager * HistoryManager::m_historyManager = 0;

HistoryManager * HistoryManager::historyManager()
{
    if(!m_historyManager)
    {
        m_historyManager = new HistoryManager();
    }
    return m_historyManager;
}

HistoryItem * HistoryManager::item(const QModelIndex & index) const
{
    return m_items.value(index.row());
}

int HistoryManager::count() const
{
    return m_items.count();
}

QModelIndex HistoryManager::contains(const QUrl & url) const
{
    QModelIndex index;
    for(int i = 0; i < count(); i++)
    {
        if(item(this->index(i))->url() == url)
        {
            index = this->index(i);
            break;
        }
    }
    return index;
}

HistoryItem * HistoryManager::addItem(const QUrl & url, const QString & title)
{
    return addItem(url, title, QDateTime::currentDateTime(), 1);
}

void HistoryManager::removeItem(const QModelIndex & index)
{
    if(item(index))
    {
        beginRemoveRows(QModelIndex(), index.row(), index.row());
        delete m_items.at(index.row());
        m_items.removeAt(index.row());
        endRemoveRows();
    }
}

int HistoryManager::columnCount(const QModelIndex & /*parent*/) const
{
    return 2;
}

int HistoryManager::rowCount(const QModelIndex & /*parent*/) const
{
    return count();
}

QVariant HistoryManager::data(const QModelIndex & index, int role) const
{
    HistoryItem * item = this->item(index);
    if(!index.isValid() || index.column() >= 2 || index.row() >= count())
    {
        return QVariant();
    }
    else if(role == Qt::EditRole && index.column() == 0)
    {
        return item->url().toString();
    }
    else if(role == Qt::DecorationRole && index.column() == 0)
    {
        return Global::notEmpty(QWebSettings::iconForUrl(item->url()));
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

QVariant HistoryManager::headerData(int section, Qt::Orientation orientation, int role) const
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

bool HistoryManager::isPrivateBrowsingEnabled() const
{
    return QWebSettings::globalSettings()->testAttribute(QWebSettings::PrivateBrowsingEnabled);
}

void HistoryManager::setPrivateBrowsingEnabled(bool enabled)
{
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PrivateBrowsingEnabled, enabled);
}

int HistoryManager::expirationDays() const
{
    return m_expirationDays;
}

void HistoryManager::setExpirationDays(int days)
{
    m_expirationDays = days;
}

void HistoryManager::load()
{
    QSettings settings;
    setPrivateBrowsingEnabled(settings.value("WebKit/privateBrowsing", false).toBool());
    setExpirationDays(settings.value("HistoryManager/expirationDays", 7).toInt());
    loadHistory();
}

void HistoryManager::save()
{
    QSettings settings;
    settings.setValue("WebKit/privateBrowsing", isPrivateBrowsingEnabled());
    settings.setValue("HistoryManager/expirationDays", expirationDays());
    saveHistory();
}

HistoryManager::HistoryManager()
{
    QWebHistoryInterface::setDefaultInterface(new HistoryInterface(this));
}

HistoryItem * HistoryManager::addItem(const QUrl & url, const QString & title,
                                      const QDateTime & visitTime, int counter)
{
    if(!isPrivateBrowsingEnabled())
    {
        QModelIndex index = contains(url);
        if(index.isValid())
        {
            counter += item(index)->counter();
            removeItem(index);
        }
        HistoryItem * item = new HistoryItem(url, title, visitTime, counter);
        beginInsertRows(QModelIndex(), 0, 0);
        m_items.prepend(item);
        endInsertRows();
        return item;
    }
    return 0;
}

void HistoryManager::loadHistory()
{
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QFile file(path + "/history.xml");
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
                        QUrl url(xml.attributes().value("href").toString());
                        QString title;
                        QDateTime visitTime = QDateTime::currentDateTime();
                        int counter = 1;
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
                                else if(xml.name() == "time")
                                {
                                    visitTime = QDateTime::fromString(xml.readElementText(), "dd.MM.yyyy hh:mm");
                                }
                                else if(xml.name() == "counter")
                                {
                                    counter = xml.readElementText().toInt();
                                }
                                else
                                {
                                    xml.skipCurrentElement();
                                }
                            }
                        }
                        if(visitTime.daysTo(QDateTime::currentDateTime()) < expirationDays())
                        {
                            addItem(url, title, visitTime, counter);
                        }
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

void HistoryManager::saveHistory()
{
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QFile file(path + "/history.xml");
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
        HistoryItem * item = m_items.at(i);
        xml.writeStartElement("item");
        xml.writeAttribute("href", item->url().toString());
        xml.writeTextElement("title", item->title());
        xml.writeTextElement("time", item->visitTime().toString("dd.MM.yyyy hh:mm"));
        xml.writeTextElement("counter", QString::number(item->counter()));
        xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();
}
