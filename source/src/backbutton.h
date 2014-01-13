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

#ifndef BACKBUTTON_H
#define BACKBUTTON_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

class Menu;

class BackButton : public QToolButton
{
    Q_OBJECT

public:
    BackButton(QWebView * webView, QWidget * parent = 0);

private:
    QWebView * m_webView;
    QAction * m_defaultAction;
    Menu * m_popupMenu;

private slots:
    void updateButton();
    void goBack();
    void updatePopupMenu();
};

#endif // BACKBUTTON_H
