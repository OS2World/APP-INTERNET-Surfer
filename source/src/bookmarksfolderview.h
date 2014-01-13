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

#ifndef BOOKMARKSFOLDERVIEW_H
#define BOOKMARKSFOLDERVIEW_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

class BookmarksFolderModel;

class BookmarksFolderView : public QTreeView
{
    Q_OBJECT

public:
    BookmarksFolderView(QWidget * parent = 0);
    QModelIndex index() const;

private:
    BookmarksFolderModel * m_model;

private slots:
    void createFolder();
};

#endif // BOOKMARKSFOLDERVIEW_H
