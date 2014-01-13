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

#include "bookmarksfiltermodel.h"

BookmarksFilterModel::BookmarksFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool BookmarksFilterModel::filterAcceptsColumn(int /*source_column*/, const QModelIndex & /*source_parent*/) const
{
    return true;
}

bool BookmarksFilterModel::filterAcceptsRow(int source_row, const QModelIndex & source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    int count = sourceModel()->rowCount(index);
    bool accept = false;
    if(!count)
    {
        for(int i = 0; i < sourceModel()->columnCount(source_parent); i++)
        {
            QModelIndex index = sourceModel()->index(source_row, i, source_parent);
            bool b = sourceModel()->data(index, Qt::DisplayRole).toString().contains(filterRegExp());
            accept |= b;
        }
        return accept;
    }
    for(int i = 0; i < count; i++)
    {
        accept |= filterAcceptsRow(i, index);
    }
    return accept;
}
