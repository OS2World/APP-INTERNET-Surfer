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

#include "global.h"

QIcon Global::notEmpty(const QIcon & icon)
{
    QIcon newIcon(icon);
    if(icon.isNull())
    {
        newIcon = QIcon::fromTheme("text-html", QIcon(":/text-html.png"));
    }
    return newIcon;
}

QString Global::notEmpty(const QString & title)
{
    QString newTitle(title);
    if(title.isEmpty())
    {
        newTitle = QString("(") + QObject::trUtf8("No name") + ")";
    }
    return newTitle;
}

QString Global::notLong(const QString & title)
{
    QString newTitle(title);
    if(title.size() > 40)
    {
        newTitle.truncate(37);
        newTitle += "...";
    }
    return newTitle;
}
