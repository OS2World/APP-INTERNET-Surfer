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

#include "authenticationdialog.h"

AuthenticationDialog::AuthenticationDialog(QWidget * parent)
    : QDialog(parent)
{
    setWindowTitle(trUtf8("Authentication required"));
    resize(300, 200);

    QVBoxLayout * vboxlayout = new QVBoxLayout(this);
    m_userLabel = new QLabel(trUtf8("User") + ":", this);
    vboxlayout->addWidget(m_userLabel);
    m_userEdit = new QLineEdit(this);
    vboxlayout->addWidget(m_userEdit);
    m_passwordLabel = new QLabel(trUtf8("Password") + ":", this);
    vboxlayout->addWidget(m_passwordLabel);
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    vboxlayout->addWidget(m_passwordEdit);
    QHBoxLayout * hboxlayout = new QHBoxLayout();
    m_okButton = new QPushButton(this);
    m_okButton->setText(trUtf8("OK"));
    m_okButton->setIcon(QIcon::fromTheme("dialog-ok", QIcon(":/dialog-ok.png")));
    hboxlayout->addWidget(m_okButton);
    m_cancelButton = new QPushButton(this);
    m_cancelButton->setText(trUtf8("Cancel"));
    m_cancelButton->setIcon(QIcon::fromTheme("dialog-cancel", QIcon(":/dialog-cancel.png")));
    hboxlayout->addWidget(m_cancelButton);
    vboxlayout->addLayout(hboxlayout);

    connect(m_okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

QString AuthenticationDialog::user()
{
    return m_userEdit->text();
}

QString AuthenticationDialog::password()
{
    return m_passwordEdit->text();
}
