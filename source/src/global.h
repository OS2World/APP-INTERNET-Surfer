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

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

class Global
{
public:
    static QIcon notEmpty(const QIcon & icon);
    static QString notEmpty(const QString & title);
    static QString notLong(const QString & title);
};

#endif // GLOBAL_H
