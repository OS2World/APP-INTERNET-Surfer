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

#include "historyview.h"
#include "historymanager.h"
#include "historyitem.h"
#include "tabmanager.h"

HistoryView::HistoryView(QWidget * parent)
        : QTreeView(parent)
{
    header()->setResizeMode(QHeaderView::Stretch);
    setRootIsDecorated(false);
    setAlternatingRowColors(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_model = new QSortFilterProxyModel(this);
    m_model->setSourceModel(HistoryManager::historyManager());
    m_model->setDynamicSortFilter(true);
    m_model->setFilterKeyColumn(-1);
    m_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    setModel(m_model);

    m_openUrlAction = new QAction(QIcon::fromTheme("tab-new", QIcon(":/tab-new.png")), trUtf8("Open in new tab"), this);
    m_openUrlAction->setShortcut(QKeySequence::Open);
    m_copyUrlAction = new QAction(QIcon::fromTheme("edit-copy", QIcon(":/edit-copy.png")), trUtf8("Copy address"), this);
    m_copyUrlAction->setShortcut(QKeySequence::Copy);
    m_removeAction = new QAction(QIcon::fromTheme("edit-delete", QIcon(":/edit-delete.png")), trUtf8("Delete"), this);
    m_removeAction->setShortcut(QKeySequence::Delete);
    setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(updateActions()));
    connect(m_openUrlAction, SIGNAL(triggered()), this, SLOT(openUrl()));
    connect(m_copyUrlAction, SIGNAL(triggered()), this, SLOT(copyUrl()));
    connect(m_removeAction, SIGNAL(triggered()), this, SLOT(remove()));
}

QSortFilterProxyModel * HistoryView::model() const
{
    return m_model;
}

void HistoryView::switchAction(QAction * action, bool state)
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

void HistoryView::updateActions()
{
    QModelIndexList selection = selectionModel()->selectedRows();
    bool notEmpty = !selection.isEmpty();
    bool single = selection.size() == 1;
    switchAction(m_openUrlAction, notEmpty);
    m_openUrlAction->setEnabled(notEmpty);
    switchAction(m_copyUrlAction, single);
    m_copyUrlAction->setEnabled(single);
    switchAction(m_removeAction, notEmpty);
    m_removeAction->setEnabled(notEmpty);
}

void HistoryView::openUrl()
{
    HistoryManager * manager = HistoryManager::historyManager();
    foreach(QModelIndex index, selectionModel()->selectedRows())
    {
       TabManager::tabManager()->addTab(manager->item(m_model->mapToSource(index))->url());
    }
}

void HistoryView::copyUrl()
{
    QModelIndex index = selectionModel()->selectedRows().first();
    HistoryManager * manager = HistoryManager::historyManager();
    QApplication::clipboard()->setText(manager->item(m_model->mapToSource(index))->url().toString());
}

void HistoryView::remove()
{
    HistoryManager * manager = HistoryManager::historyManager();
    QModelIndexList selection;
    while(!(selection = selectionModel()->selectedRows()).isEmpty())
    {
        manager->removeItem(m_model->mapToSource(selection.first()));
    }
}
