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

#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

class HistoryItem;

class HistoryManager : public QAbstractListModel
{
    Q_OBJECT

public:
    static HistoryManager * historyManager();

    HistoryItem * item(const QModelIndex & index) const;
    int count() const;
    QModelIndex contains(const QUrl & url) const;
    HistoryItem * addItem(const QUrl & url, const QString & title);
    void removeItem(const QModelIndex & index);

    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    bool isPrivateBrowsingEnabled() const;
    void setPrivateBrowsingEnabled(bool enabled);
    int expirationDays() const;
    void setExpirationDays(int days);

public slots:
    void load();
    void save();

protected:
    HistoryManager();

private:
    static HistoryManager * m_historyManager;

    QList<HistoryItem *> m_items;

    int m_expirationDays;

    HistoryItem * addItem(const QUrl & url, const QString & title, const QDateTime & visitTime, int counter);

    void loadHistory();
    void saveHistory();
};

#endif // HISTORYMANAGER_H
