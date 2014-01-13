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

#include "downloadview.h"
#include "downloadmanager.h"
#include "downloaditem.h"
#include "downloaditemdelegate.h"

DownloadView::DownloadView(QWidget * parent)
        : QTreeView(parent)
{
    header()->setResizeMode(QHeaderView::Stretch);
    setRootIsDecorated(false);
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_model = new QSortFilterProxyModel(this);
    m_model->setSourceModel(DownloadManager::downloadManager());
    m_model->setDynamicSortFilter(true);
    m_model->setFilterKeyColumn(-1);
    m_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    setModel(m_model);

    m_delegate = new DownloadItemDelegate(this);
    setItemDelegate(m_delegate);

    m_restartAction = new QAction(QIcon::fromTheme("media-playback-start", QIcon(":/media-playback-start.png")), trUtf8("Restart"), this);
    m_restartAction->setShortcut(QKeySequence::Refresh);
    m_stopAction = new QAction(QIcon::fromTheme("media-playback-stop", QIcon(":/media-playback-stop.png")), trUtf8("Stop"), this);
    m_stopAction->setShortcut(Qt::Key_F4);
    m_removeAction = new QAction(QIcon::fromTheme("edit-delete", QIcon(":/edit-delete.png")), trUtf8("Delete"), this);
    m_removeAction->setShortcut(QKeySequence::Delete);
    setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(updateActions()));
    connect(m_restartAction, SIGNAL(triggered()), this, SLOT(restart()));
    connect(m_stopAction, SIGNAL(triggered()), this, SLOT(stop()));
    connect(m_removeAction, SIGNAL(triggered()), this, SLOT(remove()));
}

QSortFilterProxyModel * DownloadView::model() const
{
    return m_model;
}

void DownloadView::switchAction(QAction * action, bool state)
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

void DownloadView::updateActions()
{
    QModelIndexList selection = selectionModel()->selectedRows();
    DownloadManager * manager = DownloadManager::downloadManager();
    bool hasActive = false;
    bool hasFinished = false;
    for(int i = 0; i < manager->count(); i++)
    {
       QModelIndex index = manager->index(i);
       DownloadItem * item = manager->item(index);
       disconnect(item, SIGNAL(started()), this, SLOT(updateActions()));
       disconnect(item, SIGNAL(finished()), this, SLOT(updateActions()));
    }
    foreach(QModelIndex index, selection)
    {
       DownloadItem * item = manager->item(m_model->mapToSource(index));
       hasActive |= !item->isFinished();
       hasFinished |= item->isFinished();
       connect(item, SIGNAL(started()), this, SLOT(updateActions()));
       connect(item, SIGNAL(finished()), this, SLOT(updateActions()));
    }
    switchAction(m_restartAction, hasActive || hasFinished);
    m_restartAction->setEnabled(!hasActive && hasFinished);
    switchAction(m_stopAction, hasActive && !hasFinished);
    m_stopAction->setEnabled(hasActive && !hasFinished);
    switchAction(m_removeAction, hasActive || hasFinished);
    m_removeAction->setEnabled(!hasActive && hasFinished);
}

void DownloadView::restart()
{
    QModelIndexList selection = selectionModel()->selectedRows();
    DownloadManager * manager = DownloadManager::downloadManager();
    foreach(QModelIndex index, selection)
    {
       manager->item(m_model->mapToSource(index))->start();
    }
}

void DownloadView::stop()
{
    QModelIndexList selection = selectionModel()->selectedRows();
    DownloadManager * manager = DownloadManager::downloadManager();
    foreach(QModelIndex index, selection)
    {
        manager->item(m_model->mapToSource(index))->stop();
    }
}

void DownloadView::remove()
{
    DownloadManager * manager = DownloadManager::downloadManager();
    QModelIndexList selection;
    while(!(selection = selectionModel()->selectedRows()).isEmpty())
    {
        manager->removeItem(m_model->mapToSource(selection.first()));
    }
}
