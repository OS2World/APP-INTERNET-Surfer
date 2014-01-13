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

#ifndef ADDRESSBAR_H
#define ADDRESSBAR_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

class AddressBar : public QLineEdit
{
    Q_OBJECT

public:
    AddressBar(QWebView * webView, QWidget * parent = 0);
    QUrl url() const;

public slots:
    void setUrl(const QUrl & url);

private:
    QWebView * m_webView;
    QShortcut * m_focusShortcut;
    QCompleter * m_completer;
    QTreeView * m_popup;

private slots:
    void load();
};

#endif // ADDRESSBAR_H
