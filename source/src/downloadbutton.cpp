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

#include "downloadbutton.h"
#include "downloadtab.h"
#include "downloadmanager.h"
#include "tabmanager.h"

int DownloadButton::m_count = 0;
DownloadTab * DownloadButton::m_tab = 0;

DownloadButton::DownloadButton(QWidget * parent)
        : QToolButton(parent)
{
    m_count++;
    if(m_count == 1)
    {
        m_tab = new DownloadTab();
    }

    setAutoRaise(true);
    setIcon(QIcon::fromTheme("download", QIcon(":/download.png")));
    setToolTip(trUtf8("Downloads"));
    setShortcut(Qt::CTRL + Qt::Key_D);

    connect(this, SIGNAL(clicked()), this, SLOT(openTab()));
    connect(DownloadManager::downloadManager(), SIGNAL(itemsChanged()), this, SLOT(openTab()));
}

DownloadButton::~DownloadButton()
{
    m_count--;
    if(!m_count)
    {
        m_tab->deleteLater();
    }
}

void DownloadButton::openTab()
{
    TabManager::tabManager()->addTab(m_tab);
}
