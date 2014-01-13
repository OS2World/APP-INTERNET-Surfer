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

#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

class DownloadItem;

class DownloadManager : public QAbstractListModel
{
    Q_OBJECT

public:
    static DownloadManager * downloadManager();
    static QString sizeString(qint64 bytes);

    DownloadItem * item(const QModelIndex & index) const;
    int count() const;
    DownloadItem * download(const QUrl & from, const QUrl & to);
    void removeItem(const QModelIndex & index);

    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    QString standardPath() const;
    void setStandardPath(const QString & path);

public slots:
    void load();
    void save();

protected:
    DownloadManager();

private:
    static DownloadManager * m_downloadManager;

    QList<DownloadItem *> m_items;

    QString m_standardPath;

private slots:
    void updateData();

signals:
    void itemsChanged();
};

#endif // DOWNLOADMANAGER_H
