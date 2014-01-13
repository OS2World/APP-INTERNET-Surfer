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

#ifndef RELOADSTOPBUTTON_H
#define RELOADSTOPBUTTON_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

class ReloadStopButton : public QToolButton
{
    Q_OBJECT

public:
    ReloadStopButton(QWebView * webView, QWidget * parent = 0);
    void setIcons(const QIcon & icon1, const QIcon & icon2);
    void setToolTips(const QString & tip1, const QString & tip2);
    void setShortcuts(const QKeySequence & key1, const QKeySequence & key2);

private:
    enum State
    {
        Reload,
        Stop
    };
    QWebView * m_webView;
    QIcon m_icon1;
    QIcon m_icon2;
    QString m_tip1;
    QString m_tip2;
    QKeySequence m_key1;
    QKeySequence m_key2;
    State state;

    void updateIcon();
    void updateToolTip();
    void updateShortcut();
    void updateAll();

private slots:
    void goToReloadState();
    void goToStopState();
    void reloadStop();
};

#endif // RELOADSTOPBUTTON_H
