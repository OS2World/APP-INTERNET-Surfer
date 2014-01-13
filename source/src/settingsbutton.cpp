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

#include "settingsbutton.h"
#include "settingsdialog.h"
#include "menu.h"

SettingsButton::SettingsButton(QWidget * parent)
        : QToolButton(parent)
{
    setAutoRaise(true);
    setIcon(QIcon::fromTheme("configure", QIcon(":/configure.png")));
    setToolTip(trUtf8("Tools"));
    setShortcut(Qt::CTRL + Qt::Key_S);
    setPopupMode(QToolButton::InstantPopup);

    m_popupMenu = new Menu(this);
    m_settingsAction = m_popupMenu->addAction(QIcon::fromTheme("configure", QIcon(":/configure.png")), trUtf8("Settings"));
    m_popupMenu->addSeparator();
    m_aboutSurferAction = m_popupMenu->addAction(qApp->windowIcon(), trUtf8("About") + " Surfer");
    m_aboutQtAction = m_popupMenu->addAction(qApp->windowIcon(), trUtf8("About") + " Qt");
    setMenu(m_popupMenu);

    connect(m_settingsAction, SIGNAL(triggered()), this, SLOT(openSettingsDialog()));
    connect(m_aboutSurferAction, SIGNAL(triggered()), this, SLOT(openAboutSurferDialog()));
    connect(m_aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void SettingsButton::openSettingsDialog()
{
    SettingsDialog dialog(this);
    dialog.exec();
}

void SettingsButton::openAboutSurferDialog()
{
    QString text = "<big><b>Surfer " + qApp->applicationVersion() + "</b></big><br><br>";
    text += "Surfer - " + trUtf8("simple web browser based on")+ " QtWebKit " + trUtf8("that focus on usability");
    text += "<br><br>Copyright (C) 2010 " + trUtf8("Andrianov Ivan");
    text += "<br><br>Surfer " + trUtf8("is distributed under terms of");
    text += " <a href=\"http://www.gnu.org/licenses/gpl.html\">GNU General Public License 3</a>";
    QMessageBox::about(this, trUtf8("About") + " Surfer", text);
}
