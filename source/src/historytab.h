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

#ifndef HISTORYTAB_H
#define HISTORYTAB_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>
#include "itab.h"

class RestoreButton;
class BookmarksButton;
class DownloadButton;
class SettingsButton;
class HistoryView;

class HistoryTab : public ITab
{
    Q_OBJECT

public:
    HistoryTab();

    QString title() const;
    QIcon icon() const;

private:
    QVBoxLayout * m_layout;

    QToolBar * m_toolBar;
    QLabel * m_searchLabel;
    QLineEdit * m_searchEdit;
    RestoreButton * m_restoreButton;
    BookmarksButton * m_bookmarksButton;
    DownloadButton * m_downloadButton;
    SettingsButton * m_settingsButton;

    HistoryView * m_view;

    void setupToolBar();
};

#endif // HISTORYTAB_H
