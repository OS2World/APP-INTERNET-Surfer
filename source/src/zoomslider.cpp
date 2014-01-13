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

#include "zoomslider.h"

ZoomSlider::ZoomSlider(QWebView * webView, QWidget * parent)
        : QSlider(parent)
        , m_webView(webView)
{
    setRange(20, 500);
    setSingleStep(10);

    connect(this, SIGNAL(valueChanged(int)), this, SLOT(setZoom(int)));

    reset();
}

void ZoomSlider::increase()
{
    setValue(value() + singleStep());
}

void ZoomSlider::decrease()
{
    setValue(value() - singleStep());
}

void ZoomSlider::reset()
{
    setValue(100);
}

void ZoomSlider::setZoom(int value)
{
    QWidget::setToolTip(trUtf8("Zoom") + ": " + QString::number(value) + "%");
    m_webView->setZoomFactor(value / 100.0);
}
