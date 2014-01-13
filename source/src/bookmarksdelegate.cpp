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

#include "bookmarksdelegate.h"

BookmarksDelegate::BookmarksDelegate(QObject * parent)
        : QStyledItemDelegate(parent)
{
}

void BookmarksDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
    QString text = index.data(Qt::DisplayRole).toString();
    QLineEdit * lineEdit = qobject_cast<QLineEdit *>(editor);
    if(lineEdit)
    {
        lineEdit->setText(text);
        lineEdit->selectAll();
    }
}

void BookmarksDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
    QLineEdit * lineEdit = qobject_cast<QLineEdit *>(editor);
    if(lineEdit)
    {
        model->setData(index, lineEdit->text(), Qt::DisplayRole);
    }
}

void BookmarksDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option,
                                             const QModelIndex & /*index*/) const
{
    editor->setGeometry(option.rect);
}
