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

#include "downloaditemdelegate.h"

DownloadItemDelegate::DownloadItemDelegate(QObject * parent)
        : QStyledItemDelegate(parent)
{
}

void DownloadItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option,
                                 const QModelIndex & index) const
{
    if(index.column() == 2)
    {
        QStyleOptionProgressBar progressBarOption;
        progressBarOption.rect = option.rect;
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.progress = index.data(Qt::UserRole).toInt();
        progressBarOption.text = index.data().toString();
        progressBarOption.textVisible = true;

        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}
