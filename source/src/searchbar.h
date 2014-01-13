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

#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

class SearchBar : public QWidget
{
    Q_OBJECT

public:
    SearchBar(QWebView * webView, QWidget * parent = 0);

private:
    QWebView * m_webView;

    QShortcut * m_focusShortcut;
    QHBoxLayout * m_layout;
    QLineEdit * m_lineEdit;

private slots:
    void getSelection();
    void changeSearchData();
};

#endif // SEARCHBAR_H
