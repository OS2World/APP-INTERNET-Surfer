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

#ifndef DOWNLOADITEM_H
#define DOWNLOADITEM_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

class DownloadItem : public QObject
{
    Q_OBJECT

public:
    DownloadItem(const QUrl & from, const QUrl & to, QObject * parent = 0);
    bool isFinished() const;
    int progress() const;
    qint64 received() const;
    qint64 total() const;
    QUrl from() const;
    QUrl to() const;

public slots:
    void start();
    void stop();

private:
    bool m_isFinished;
    int m_progress;
    qint64 m_received;
    qint64 m_total;
    QUrl m_from;
    QUrl m_to;

    QNetworkReply * m_reply;
    QFile m_file;

private slots:
    void afterFinish();
    void setProgress(qint64 received, qint64 total);

signals:
    void started();
    void progressChanged(int progress);
    void finished();
};

#endif // DOWNLOADITEM_H
