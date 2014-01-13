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

#include "downloadtab.h"
#include "restorebutton.h"
#include "historybutton.h"
#include "bookmarksbutton.h"
#include "settingsbutton.h"
#include "downloadview.h"

DownloadTab::DownloadTab()
{
    m_layout = new QVBoxLayout(this);
    m_layout->setMargin(0);
    m_layout->setSpacing(0);

    setupToolBar();

    m_view = new DownloadView(this);
    m_layout->addWidget(m_view, 1);

    connect(m_searchEdit, SIGNAL(textChanged(const QString &)),
            m_view->model(), SLOT(setFilterFixedString(const QString &)));
}

QString DownloadTab::title() const
{
    return trUtf8("Downloads");
}

QIcon DownloadTab::icon() const
{
    return QIcon::fromTheme("download", QIcon(":/download.png"));
}

void DownloadTab::setupToolBar()
{
    m_toolBar = new QToolBar(this);
    m_searchLabel = new QLabel(trUtf8("Search"));
    m_toolBar->addWidget(m_searchLabel);
    m_searchEdit = new QLineEdit();
    m_toolBar->addWidget(m_searchEdit);
    m_restoreButton = new RestoreButton();
    m_toolBar->addWidget(m_restoreButton);
    m_historyButton = new HistoryButton();
    m_toolBar->addWidget(m_historyButton);
    m_bookmarksButton = new BookmarksButton(0);
    m_toolBar->addWidget(m_bookmarksButton);
    m_settingsButton = new SettingsButton();
    m_toolBar->addWidget(m_settingsButton);
    m_layout->addWidget(m_toolBar);
}
