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

#ifndef HISTORYITEM_H
#define HISTORYITEM_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

class HistoryItem
{
public:
    HistoryItem(const QUrl & url, const QString & title, const QDateTime & visitTime, int counter);
    QUrl url() const;
    QString title() const;
    QDateTime visitTime() const;
    int counter() const;

private:
    QUrl m_url;
    QString m_title;
    QDateTime m_visitTime;
    int m_counter;
};

#endif // HISTORYITEM_H
