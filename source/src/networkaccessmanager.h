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

#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

class NetworkCookieJar;

class NetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT

public:
    static NetworkAccessManager * networkAccessManager();

    void addTrustedDomain(const QUrl & url);
    void addTempTrustedDomain(const QUrl & url);
    bool isTrustedDomain(const QUrl & url) const;
    void addTempRejectedDomain(const QUrl & url);
    bool isRejectedDomain(const QUrl & url) const;

    qint64 cacheSize() const;
    void setCacheSize(qint64 size);
    QUrl proxy() const;
    void setProxy(const QUrl & url);

public slots:
    void load();
    void save();

protected:
    NetworkAccessManager();

private:
    static NetworkAccessManager * m_networkAccessManager;

    QNetworkDiskCache * m_cache;
    NetworkCookieJar * m_cookieJar;

    QList<QUrl> m_trustedDomains;
    QList<QUrl> m_tempTrustedDomains;
    QList<QUrl> m_tempRejectedDomains;

private slots:
    void authenticate(QNetworkReply * reply, QAuthenticator * authenticator);
    void authenticate(const QNetworkProxy & proxy, QAuthenticator * authenticator);
    void handleSslErrors(QNetworkReply * reply, const QList<QSslError> & errors);
};

#endif // NETWORKACCESSMANAGER_H
