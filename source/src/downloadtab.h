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

#ifndef DOWNLOADTAB_H
#define DOWNLOADTAB_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>
#include "itab.h"

class RestoreButton;
class HistoryButton;
class BookmarksButton;
class SettingsButton;
class DownloadView;

class DownloadTab : public ITab
{
    Q_OBJECT

public:
    DownloadTab();

    QString title() const;
    QIcon icon() const;

private:
    QVBoxLayout * m_layout;

    QToolBar * m_toolBar;
    QLabel * m_searchLabel;
    QLineEdit * m_searchEdit;
    RestoreButton * m_restoreButton;
    HistoryButton * m_historyButton;
    BookmarksButton * m_bookmarksButton;
    SettingsButton * m_settingsButton;

    DownloadView * m_view;

    void setupToolBar();
};

#endif // DOWNLOADTAB_H
