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

#ifndef DOWNLOADVIEW_H
#define DOWNLOADVIEW_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

class DownloadItemDelegate;

class DownloadView : public QTreeView
{
    Q_OBJECT

public:
    DownloadView(QWidget * parent = 0);
    QSortFilterProxyModel * model() const;

private:
    QSortFilterProxyModel * m_model;

    DownloadItemDelegate * m_delegate;

    QAction * m_restartAction;
    QAction * m_stopAction;
    QAction * m_removeAction;

    void switchAction(QAction * action, bool state);

private slots:
    void updateActions();
    void restart();
    void stop();
    void remove();
};

#endif // DOWNLOADVIEW_H
