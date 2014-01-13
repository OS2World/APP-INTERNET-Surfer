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

#include "downloaditem.h"
#include "networkaccessmanager.h"

DownloadItem::DownloadItem(const QUrl & from, const QUrl & to, QObject * parent)
        : QObject(parent)
        , m_isFinished(false)
        , m_progress(0)
        , m_received(0)
        , m_total(0)
        , m_from(from)
        , m_to(to)
        , m_reply(0)
        , m_file(to.toString())
{
}

bool DownloadItem::isFinished() const
{
    return m_isFinished;
}

int DownloadItem::progress() const
{
    return m_progress;
}

qint64 DownloadItem::received() const
{
    return m_received;
}

qint64 DownloadItem::total() const
{
    return m_total;
}

QUrl DownloadItem::from() const
{
    return m_from;
}

QUrl DownloadItem::to() const
{
    return m_to;
}

void DownloadItem::start()
{
    stop();

    if(!m_file.open(QIODevice::WriteOnly))
    {
        return;
    }

    m_isFinished = false;
    m_progress = 0;
    m_received = 0;
    m_total = 0;
    emit started();

    m_reply = NetworkAccessManager::networkAccessManager()->get(QNetworkRequest(m_from));

    connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(setProgress(qint64, qint64)));
    connect(m_reply, SIGNAL(finished()), this, SLOT(afterFinish()));
}

void DownloadItem::stop()
{
    if(m_reply)
    {
        m_reply->abort();
    }
}

void DownloadItem::afterFinish()
{
    if(m_reply->error() == QNetworkReply::NoError)
    {
        m_received = m_total;
        m_progress = 100;
        emit progressChanged(m_progress);
    }
    m_isFinished = true;
    emit finished();
    m_file.write(m_reply->readAll());
    m_file.close();
    m_reply = 0;
}

void DownloadItem::setProgress(qint64 received, qint64 total)
{
    if(total)
    {
        m_progress = received * 100 / total;
    }
    else
    {
        m_progress = 0;
    }
    m_received = received;
    m_total = total;
    emit progressChanged(m_progress);
    m_file.write(m_reply->readAll());
}
