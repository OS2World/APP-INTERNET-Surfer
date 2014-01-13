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

#include "menu.h"

Menu::Menu(QWidget * parent)
    : QMenu(parent)
{
}

Menu * Menu::addMenu(const QIcon & icon, const QString & title)
{
    Menu * menu = new Menu(this);
    menu->setTitle(title);
    menu->setIcon(icon);
    QMenu::addMenu(menu);
    return menu;
}

bool Menu::event(QEvent * event)
{
    if(event->type() == QEvent::ToolTip)
    {
        QHelpEvent * helpEvent = static_cast<QHelpEvent *>(event);
        QAction * action = actionAt(helpEvent->pos());
        if(action)
        {
            QString toolTip = action->toolTip();
            if(toolTip != action->text() && !toolTip.isEmpty())
            {
                QToolTip::showText(helpEvent->globalPos(), toolTip);
                event->accept();
                return true;
            }
        }
        QToolTip::hideText();
        event->ignore();
        return true;
    }
    return QMenu::event(event);
}
