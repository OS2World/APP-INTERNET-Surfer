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

#ifndef ZOOMSLIDER_H
#define ZOOMSLIDER_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

class ZoomSlider : public QSlider
{
    Q_OBJECT

public:
    ZoomSlider(QWebView * webView, QWidget * parent = 0);

public slots:
    void increase();
    void decrease();
    void reset();

private:
    QWebView * m_webView;

private slots:
    void setZoom(int value);
};

#endif // ZOOMSLIDER_H
