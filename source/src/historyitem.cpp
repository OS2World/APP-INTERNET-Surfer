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

#include "historyitem.h"

HistoryItem::HistoryItem(const QUrl & url, const QString & title, const QDateTime & visitTime, int counter)
        : m_url(url)
        , m_title(title)
        , m_visitTime(visitTime)
        , m_counter(counter)
{
}

QUrl HistoryItem::url() const
{
    return m_url;
}

QString HistoryItem::title() const
{
    return m_title;
}

QDateTime HistoryItem::visitTime() const
{
    return m_visitTime;
}

int HistoryItem::counter() const
{
    return m_counter;
}
