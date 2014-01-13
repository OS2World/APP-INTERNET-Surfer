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

#include "zoombar.h"
#include "zoomslider.h"

ZoomBar::ZoomBar(QWebView * webView, QWidget * parent)
    : QToolBar(parent)
{
    setIconSize(QSize(16,16));

    m_zoomOutButton = new QToolButton();
    m_zoomOutButton->setAutoRaise(true);
    m_zoomOutButton->setIcon(QIcon::fromTheme("zoom-out", QIcon(":/zoom-out.png")));
    m_zoomOutButton->setToolTip(trUtf8("Zoom out"));
    m_zoomOutButton->setShortcut(QKeySequence::ZoomOut);
    addWidget(m_zoomOutButton);
    m_zoomSlider = new ZoomSlider(webView);
    m_zoomSlider->setOrientation(Qt::Horizontal);
    m_zoomSlider->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    addWidget(m_zoomSlider);
    m_zoomInButton = new QToolButton();
    m_zoomInButton->setAutoRaise(true);
    m_zoomInButton->setIcon(QIcon::fromTheme("zoom-in", QIcon(":/zoom-in.png")));
    m_zoomInButton->setToolTip(trUtf8("Zoom in"));
    m_zoomInButton->setShortcut(QKeySequence::ZoomIn);
    addWidget(m_zoomInButton);

    m_zoomOriginalButton = new QToolButton();
    m_zoomOriginalButton->setAutoRaise(true);
    m_zoomOriginalButton->setIcon(QIcon::fromTheme("zoom-original", QIcon(":/zoom-original.png")));
    m_zoomOriginalButton->setToolTip(trUtf8("Original zoom"));
    m_zoomOriginalButton->setShortcut(Qt::CTRL + Qt::Key_0);
    addWidget(m_zoomOriginalButton);

    connect(m_zoomOutButton, SIGNAL(clicked()), m_zoomSlider, SLOT(decrease()));
    connect(m_zoomInButton, SIGNAL(clicked()), m_zoomSlider, SLOT(increase()));
    connect(m_zoomOriginalButton, SIGNAL(clicked()), m_zoomSlider, SLOT(reset()));
}
