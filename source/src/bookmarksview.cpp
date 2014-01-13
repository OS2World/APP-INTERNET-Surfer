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

#include "bookmarksview.h"
#include "tabmanager.h"
#include "bookmarksmanager.h"
#include "bookmarksitem.h"
#include "bookmarksfiltermodel.h"
#include "bookmarksdelegate.h"

BookmarksView::BookmarksView(QWidget * parent)
        : QTreeView(parent)
{
    header()->setResizeMode(QHeaderView::Stretch);
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_model = new BookmarksFilterModel(this);
    m_model->setSourceModel(BookmarksManager::bookmarksManager());
    m_model->setDynamicSortFilter(true);
    m_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    setModel(m_model);

    m_delegate = new BookmarksDelegate(this);
    setItemDelegate(m_delegate);

    m_openUrlAction = new QAction(QIcon::fromTheme("tab-new", QIcon(":/tab-new.png")), trUtf8("Open in new tab"), this);
    m_openUrlAction->setShortcut(QKeySequence::Open);
    m_createFolderAction = new QAction(QIcon::fromTheme("folder-new", QIcon(":/folder-new.png")), trUtf8("Create folder"), this);
    m_createFolderAction->setShortcut(QKeySequence::New);
    m_editTitleAction = new QAction(QIcon::fromTheme("edit-rename", QIcon(":/edit-rename.png")), trUtf8("Edit title"), this);
    m_editTitleAction->setShortcut(Qt::Key_F2);
    m_editUrlAction = new QAction(QIcon::fromTheme("edit-rename", QIcon(":/edit-rename.png")), trUtf8("Edit address"), this);
    m_editUrlAction->setShortcut(Qt::Key_F3);
    m_copyUrlAction = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/edit-copy.png")), trUtf8("Copy address"), this);
    m_copyUrlAction->setShortcut(QKeySequence::Copy);
    m_removeAction = new QAction(QIcon::fromTheme("edit-delete", QIcon(":/edit-delete.png")), trUtf8("Delete"), this);
    m_removeAction->setShortcut(QKeySequence::Delete);
    setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(updateActions()));
    connect(m_openUrlAction, SIGNAL(triggered()), this, SLOT(openUrl()));
    connect(m_createFolderAction, SIGNAL(triggered()), this, SLOT(createFolder()));
    connect(m_editTitleAction, SIGNAL(triggered()), this, SLOT(editTitle()));
    connect(m_editUrlAction, SIGNAL(triggered()), this, SLOT(editUrl()));
    connect(m_copyUrlAction, SIGNAL(triggered()), this, SLOT(copyUrl()));
    connect(m_removeAction, SIGNAL(triggered()), this, SLOT(remove()));

    expandAll();
}

QSortFilterProxyModel * BookmarksView::model() const
{
    return m_model;
}

void BookmarksView::switchAction(QAction * action, bool state)
{
    if(state)
    {
        addAction(action);
    }
    else
    {
        removeAction(action);
    }
}

void BookmarksView::updateActions()
{
    BookmarksManager * manager = BookmarksManager::bookmarksManager();
    QModelIndexList selection = selectionModel()->selectedRows();
    bool hasRootFolder = false;
    bool hasFolders = false;
    bool hasBookmarks = false;
    bool single = selection.size() == 1;
    foreach(QModelIndex index, selection)
    {
        BookmarksItem * item = manager->item(m_model->mapToSource(index));
        hasRootFolder |= !index.parent().isValid();
        hasFolders |= item->url().isEmpty() && index.parent().isValid();
        hasBookmarks |= !item->url().isEmpty();
    }
    bool openUrlCondition = !hasRootFolder && !hasFolders && hasBookmarks;
    bool createFolderCondition = ((!hasRootFolder && hasFolders) || (hasRootFolder && !hasFolders)) &&
                                 !hasBookmarks && single;
    bool editTitleCondition = !hasRootFolder && ((!hasFolders && hasBookmarks) || (hasFolders && !hasBookmarks)) &&
                              single;
    bool editUrlCondition = !hasRootFolder && !hasFolders && hasBookmarks && single;
    bool copyUrlCondition = !hasRootFolder && !hasFolders && hasBookmarks && single;
    bool removeCondition = !hasRootFolder && (hasFolders || hasBookmarks);
    switchAction(m_openUrlAction, openUrlCondition);
    m_openUrlAction->setEnabled(openUrlCondition);
    switchAction(m_createFolderAction, createFolderCondition);
    m_createFolderAction->setEnabled(createFolderCondition);
    switchAction(m_editTitleAction, editTitleCondition);
    m_editTitleAction->setEnabled(editTitleCondition);
    switchAction(m_editUrlAction, editUrlCondition);
    m_editUrlAction->setEnabled(editUrlCondition);
    switchAction(m_copyUrlAction, copyUrlCondition);
    m_copyUrlAction->setEnabled(copyUrlCondition);
    switchAction(m_removeAction, removeCondition);
    m_removeAction->setEnabled(removeCondition);
}

void BookmarksView::openUrl()
{
    BookmarksManager * manager = BookmarksManager::bookmarksManager();
    foreach(QModelIndex index, selectionModel()->selectedRows())
    {
        TabManager::tabManager()->addTab(manager->item(m_model->mapToSource(index))->url());
    }
}

void BookmarksView::createFolder()
{
    BookmarksManager * manager = BookmarksManager::bookmarksManager();
    QModelIndex index = m_model->mapToSource(selectionModel()->selectedRows().first());
    bool ok;
    QString title = QInputDialog::getText(this, trUtf8("Create bookmarks folder"), trUtf8("Title") + ":",
                                              QLineEdit::Normal, QString(), &ok);
    if(ok && !title.isEmpty())
    {
        manager->addItem(title, QUrl(), index);
    }
}

void BookmarksView::editTitle()
{
    edit(selectionModel()->selectedRows().first());
}

void BookmarksView::editUrl()
{
    edit(selectionModel()->selectedRows(1).first());
}

void BookmarksView::copyUrl()
{
    BookmarksManager * manager = BookmarksManager::bookmarksManager();
    QModelIndex index = m_model->mapToSource(selectionModel()->selectedRows().first());
    QApplication::clipboard()->setText(manager->item(index)->url().toString());
}

void BookmarksView::remove()
{
    BookmarksManager * manager = BookmarksManager::bookmarksManager();
    QModelIndexList selection;
    while(!(selection = selectionModel()->selectedRows()).isEmpty())
    {
        manager->removeItem(m_model->mapToSource(selection.first()));
    }
}
