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

#include "reloadstopbutton.h"

ReloadStopButton::ReloadStopButton(QWebView * webView, QWidget * parent)
        : QToolButton(parent)
        , m_webView(webView)
        , state(Reload)
{
    setAutoRaise(true);
    setIcons(QIcon::fromTheme("view-refresh", QIcon(":/view-refresh.png")), QIcon::fromTheme("process-stop"));
    setToolTips(trUtf8("Reload"), trUtf8("Stop"));
    setShortcuts(QKeySequence::Refresh, Qt::Key_F4);

    connect(m_webView, SIGNAL(loadStarted()), this, SLOT(goToStopState()));
    connect(m_webView, SIGNAL(loadProgress(int)), this, SLOT(goToStopState()));
    connect(m_webView, SIGNAL(loadFinished(bool)), this, SLOT(goToReloadState()));
    connect(this, SIGNAL(clicked()), this, SLOT(reloadStop()));
}

void ReloadStopButton::setIcons(const QIcon & icon1, const QIcon & icon2)
{
    m_icon1 = icon1;
    m_icon2 = icon2;
    updateIcon();
}

void ReloadStopButton::setToolTips(const QString & tip1, const QString & tip2)
{
    m_tip1 = tip1;
    m_tip2 = tip2;
    updateToolTip();
}
void ReloadStopButton::setShortcuts(const QKeySequence & key1, const QKeySequence & key2)
{
    m_key1 = key1;
    m_key2 = key2;
    updateShortcut();
}

void ReloadStopButton::updateIcon()
{
    if(state == Reload)
    {
        setIcon(m_icon1);
    }
    else
    {
        setIcon(m_icon2);
    }
}

void ReloadStopButton::updateToolTip()
{
    if(state == Reload)
    {
        setToolTip(m_tip1);
    }
    else
    {
        setToolTip(m_tip2);
    }

}

void ReloadStopButton::updateShortcut()
{
    if(state == Reload)
    {
        setShortcut(m_key1);
    }
    else
    {
        setShortcut(m_key2);
    }
}

void ReloadStopButton::updateAll()
{
    updateIcon();
    updateToolTip();
    updateShortcut();
}

void ReloadStopButton::goToReloadState()
{
    state = Reload;
    updateAll();
}

void ReloadStopButton::goToStopState()
{
    state = Stop;
    updateAll();
}

void ReloadStopButton::reloadStop()
{
    if(state == Reload)
    {
        m_webView->reload();
    }
    else
    {
        m_webView->stop();
    }
}
