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

#include "networkcookiejar.h"

NetworkCookieJar::NetworkCookieJar(QObject * parent)
        : QNetworkCookieJar(parent)
{
}

void NetworkCookieJar::load()
{
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation) +
                   "/networkaccessmanager";
    QFile file(path + "/cookies.xml");
    if(!QDir().mkpath(path) || !file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QXmlStreamReader xml(&file);
    QList<QNetworkCookie> cookies;
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
                        cookies += QNetworkCookie::parseCookies(xml.readElementText().toUtf8());
                    }
                    else
                    {
                        xml.skipCurrentElement();
                    }
                }
            }
        }
    }
    setAllCookies(cookies);
    file.close();
}

void NetworkCookieJar::save()
{
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation) +
                   "/networkaccessmanager";
    QFile file(path + "/cookies.xml");
    if(!QDir().mkpath(path) || !file.open(QIODevice::WriteOnly))
    {
        return;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("surfer");
    QList<QNetworkCookie> cookies = allCookies();
    foreach(QNetworkCookie cookie, cookies)
    {
        xml.writeTextElement("item", QString(cookie.toRawForm(QNetworkCookie::Full)));
    }
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();
}
