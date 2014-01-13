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

#ifndef HISTORYBUTTON_H
#define HISTORYBUTTON_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

class HistoryTab;
class Menu;

class HistoryButton : public QToolButton
{
    Q_OBJECT

public:
    HistoryButton(QWidget * parent = 0);
    ~HistoryButton();

private:
    static int m_count;
    static HistoryTab * m_tab;

    Menu * m_popupMenu;
    QAction * m_openTabAction;

private slots:
    void openTab();
    void openUrl();
    void updatePopupMenu();
};

#endif // HISTORYBUTTON_H
