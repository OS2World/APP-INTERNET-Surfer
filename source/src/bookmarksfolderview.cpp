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

#include "bookmarksfolderview.h"
#include "bookmarksfoldermodel.h"
#include "bookmarksmanager.h"

BookmarksFolderView::BookmarksFolderView(QWidget * parent)
        : QTreeView(parent)
{
    setHeaderHidden(true);
    setAlternatingRowColors(true);
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_model = new BookmarksFolderModel(this);
    m_model->setSourceModel(BookmarksManager::bookmarksManager());
    m_model->setDynamicSortFilter(true);
    setModel(m_model);
    setCurrentIndex(model()->index(0, 0, QModelIndex()));
    expand(model()->index(0, 0, QModelIndex()));
}

QModelIndex BookmarksFolderView::index() const
{
    return m_model->mapToSource(currentIndex());
}

void BookmarksFolderView::createFolder()
{
    BookmarksManager * manager = BookmarksManager::bookmarksManager();
    QModelIndex index = this->index();
    if(index.isValid())
    {
        bool ok;
        QString title = QInputDialog::getText(this, trUtf8("Create bookmarks folder"), trUtf8("Title") + ":",
                                              QLineEdit::Normal, QString(), &ok);
        if(ok && !title.isEmpty())
        {
            manager->addItem(title, QUrl(), index);
        }
    }
}
