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

#include "bookmarksfolderdialog.h"
#include "bookmarksfolderview.h"

BookmarksFolderDialog::BookmarksFolderDialog(const QString & title, QWidget * parent)
    : QDialog(parent)
{
    setWindowTitle(trUtf8("Add bookmark"));
    resize(300, 300);

    QVBoxLayout * vboxlayout = new QVBoxLayout(this);
    m_titleLabel = new QLabel(trUtf8("Title") + ":", this);
    vboxlayout->addWidget(m_titleLabel, 0);
    m_titleEdit = new QLineEdit(title, this);
    vboxlayout->addWidget(m_titleEdit, 0);
    m_folderLabel = new QLabel(trUtf8("Folder") + ":");
    vboxlayout->addWidget(m_folderLabel, 0);
    m_view = new BookmarksFolderView(this);
    vboxlayout->addWidget(m_view, 1);
    QHBoxLayout * hboxlayout = new QHBoxLayout();
    m_createFolderButton = new QPushButton(QIcon::fromTheme("folder-new", QIcon(":/folder-new.png")), trUtf8("Create folder"), this);
    hboxlayout->addWidget(m_createFolderButton, 0);
    hboxlayout->addStretch(1);
    m_okButton = new QPushButton(QIcon::fromTheme("dialog-ok", QIcon(":/dialog-ok.png")), trUtf8("OK"), this);
    hboxlayout->addWidget(m_okButton, 0);
    m_cancelButton = new QPushButton(QIcon::fromTheme("dialog-cancel", QIcon(":/dialog-cancel.png")), trUtf8("Cancel"), this);
    hboxlayout->addWidget(m_cancelButton, 0);
    vboxlayout->addLayout(hboxlayout, 0);

    m_titleEdit->selectAll();

    connect(m_createFolderButton, SIGNAL(clicked()), m_view, SLOT(createFolder()));
    connect(m_okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

QString BookmarksFolderDialog::title() const
{
    return m_titleEdit->text();
}

QModelIndex BookmarksFolderDialog::index() const
{
    return m_view->index();
}
