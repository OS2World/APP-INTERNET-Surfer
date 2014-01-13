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

#include "networkaccessmanager.h"
#include "networkcookiejar.h"
#include "authenticationdialog.h"

NetworkAccessManager * NetworkAccessManager::m_networkAccessManager = 0;

NetworkAccessManager * NetworkAccessManager::networkAccessManager()
{
    if(!m_networkAccessManager)
    {
        m_networkAccessManager = new NetworkAccessManager();
    }
    return m_networkAccessManager;
}

void NetworkAccessManager::addTrustedDomain(const QUrl & url)
{
    QUrl newUrl;
    newUrl.setScheme(url.scheme());
    newUrl.setHost(url.host());
    m_trustedDomains += newUrl;
}

void NetworkAccessManager::addTempTrustedDomain(const QUrl & url)
{
    QUrl newUrl;
    newUrl.setScheme(url.scheme());
    newUrl.setHost(url.host());
    m_tempTrustedDomains += newUrl;
}

bool NetworkAccessManager::isTrustedDomain(const QUrl & url) const
{
    for(int i = 0; i < m_trustedDomains.count(); i++)
    {
        if(m_trustedDomains.at(i).host() == url.host())
        {
            return true;
        }
    }
    for(int i = 0; i < m_tempTrustedDomains.count(); i++)
    {
        if(m_tempTrustedDomains.at(i).host() == url.host())
        {
            return true;
        }
    }
    return false;
}

void NetworkAccessManager::addTempRejectedDomain(const QUrl & url)
{
    QUrl newUrl;
    newUrl.setScheme(url.scheme());
    newUrl.setHost(url.host());
    m_tempRejectedDomains += newUrl;
}

bool NetworkAccessManager::isRejectedDomain(const QUrl & url) const
{
    for(int i = 0; i < m_tempRejectedDomains.count(); i++)
    {
        if(m_tempRejectedDomains.at(i).host() == url.host())
        {
            return true;
        }
    }
    return false;
}

qint64 NetworkAccessManager::cacheSize() const
{
    return m_cache->maximumCacheSize();
}

void NetworkAccessManager::setCacheSize(qint64 size)
{
    m_cache->setMaximumCacheSize(size);
}

QUrl NetworkAccessManager::proxy() const
{
    QNetworkProxy proxy = QNetworkAccessManager::proxy();
    QUrl url;
    if(proxy.type() == QNetworkProxy::Socks5Proxy)
    {
        url.setHost(proxy.hostName());
        url.setPort(proxy.port());
        url.setUserName(proxy.user());
        url.setPassword(proxy.password());
    }
    return url;
}

void NetworkAccessManager::setProxy(const QUrl & url)
{
    QNetworkProxy proxy;
    if(url.isEmpty())
    {
        proxy.setType(QNetworkProxy::NoProxy);
    }
    else
    {
        proxy.setType(QNetworkProxy::Socks5Proxy);
        proxy.setHostName(url.host());
        proxy.setPort(url.port());
        proxy.setUser(url.userName());
        proxy.setPassword(url.password());
    }
    QNetworkAccessManager::setProxy(proxy);
}

void NetworkAccessManager::load()
{
    m_cookieJar->load();
    QSettings settings;
    setCacheSize(settings.value("NetworkAccessManager/cacheSize", 50 * 1024 * 1024).toLongLong());
    setProxy(settings.value("NetworkAccessManager/proxy", QUrl()).toUrl());
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation) +
                   "/networkaccessmanager";
    QFile file(path + "/trusteddomains.xml");
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
                        while(!xml.atEnd())
                        {
                            xml.readNext();
                            if(xml.isEndElement())
                            {
                                break;
                            }
                            if(xml.isStartElement())
                            {
                                xml.skipCurrentElement();
                            }
                        }
                        addTrustedDomain(url);
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

void NetworkAccessManager::save()
{
    m_cookieJar->save();
    QSettings settings;
    settings.setValue("NetworkAccessManager/cacheSize", cacheSize());
    settings.setValue("NetworkAccessManager/proxy", proxy());
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation) +
                   "/networkaccessmanager";
    QFile file(path + "/trusteddomains.xml");
    if(!QDir().mkpath(path) || !file.open(QIODevice::WriteOnly))
    {
        return;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("surfer");
    for(int i = 0; i < m_trustedDomains.count(); i++)
    {
        xml.writeStartElement("item");
        xml.writeAttribute("href", m_trustedDomains.at(i).toString());
        xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();
}

NetworkAccessManager::NetworkAccessManager()
{
    m_cache = new QNetworkDiskCache();
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation) +
                   "/networkaccessmanager/cache";
    m_cache->setCacheDirectory(path);
    setCache(m_cache);
    m_cookieJar = new NetworkCookieJar();
    setCookieJar(m_cookieJar);

    connect(this, SIGNAL(authenticationRequired(QNetworkReply *, QAuthenticator *)),
            this, SLOT(authenticate(QNetworkReply *, QAuthenticator *)));
    connect(this, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)),
            this, SLOT(authenticate(const QNetworkProxy &, QAuthenticator *)));
    connect(this, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError> &)),
            this, SLOT(handleSslErrors(QNetworkReply *, const QList<QSslError> &)));
}

void NetworkAccessManager::authenticate(QNetworkReply * /*reply*/, QAuthenticator * authenticator)
{
    AuthenticationDialog dialog;
    if(dialog.exec() == QDialog::Accepted)
    {
        authenticator->setUser(dialog.user());
        authenticator->setPassword(dialog.password());
    }
}

void NetworkAccessManager::authenticate(const QNetworkProxy & /*proxy*/, QAuthenticator * authenticator)
{
    authenticate(0, authenticator);
}

void NetworkAccessManager::handleSslErrors(QNetworkReply * reply, const QList<QSslError> & errors)
{
    NetworkAccessManager * manager = NetworkAccessManager::networkAccessManager();
    QUrl url = reply->url();
    if(manager->isTrustedDomain(url))
    {
        reply->ignoreSslErrors();
    }
    else if(manager->isRejectedDomain(url))
    {
        reply->abort();
        QString text = trUtf8("Page")  + " \"" + url.toString() + "\" ";
        text += trUtf8("can't be loaded because it's in reject list") + ".";
        QMessageBox::warning(0, trUtf8("Security error"), text);
    }
    else
    {
        QString text = trUtf8("Certificate of page") + " \"" + url.toString() + "\" ";
        text += trUtf8("is incorrect") +".\n";
        text += trUtf8("Visiting this page may cause damage to your computer") + "!\n\n";
        text += trUtf8("Do you really trust this page") + "?";
        QString detailedText;
        for(int i = 0; i < errors.count(); i++)
        {
            QSslCertificate certificate = errors.at(i).certificate();
            QString issuerName = certificate.issuerInfo(QSslCertificate::CommonName);
            QString subjectName = certificate.subjectInfo(QSslCertificate::CommonName);
            detailedText += "----------------------------------------";
            detailedText += trUtf8("Error") + QString(" #%1").arg(i + 1);
            detailedText += "---------------------------------------\n";
            detailedText += trUtf8("Certificate issuer") + ": \"" + issuerName + "\"\n";
            detailedText += trUtf8("Certificate subject") + ": \"" + subjectName + "\"\n";
            detailedText += trUtf8("Error") + ": \"" + errors.at(i).errorString() + "\"\n";
            detailedText += "----------------------------------------------";
            detailedText += "------------------------------------------\n";
        }
        QMessageBox question(QMessageBox::Question, trUtf8("Security error"), text);
        question.setDetailedText(detailedText);
        QPushButton * trustButton = new QPushButton(QIcon::fromTheme("dialog-ok", QIcon(":/dialog-ok.png")),
                                                    trUtf8("Only for this session"), &question);
        question.addButton(trustButton, QMessageBox::AcceptRole);
        QPushButton * trustAlwaysButton = new QPushButton(QIcon::fromTheme("dialog-ok", QIcon(":/dialog-ok.png")),
                                                          trUtf8("Always"), &question);
        question.addButton(trustAlwaysButton, QMessageBox::AcceptRole);
        QPushButton * noButton = new QPushButton(QIcon::fromTheme("dialog-cancel", QIcon(":/dialog-cancel.png")), trUtf8("No"), &question);
        question.addButton(noButton, QMessageBox::RejectRole);
        question.exec();
        if(question.clickedButton() == trustButton)
        {
            reply->ignoreSslErrors();
            manager->addTempTrustedDomain(url);
        }
        else if(question.clickedButton() == trustAlwaysButton)
        {
            reply->ignoreSslErrors();
            manager->addTrustedDomain(url);
        }
        else
        {
            reply->abort();
            manager->addTempRejectedDomain(url);
        }
    }
}
