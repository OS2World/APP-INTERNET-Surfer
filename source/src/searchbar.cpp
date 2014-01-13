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

#include "searchbar.h"

SearchBar::SearchBar(QWebView * webView, QWidget * parent)
        : QWidget(parent)
        , m_webView(webView)
{
    m_focusShortcut = new QShortcut(QKeySequence::Find, this);

    m_layout = new QHBoxLayout(this);
    m_layout->setMargin(0);
    m_layout->setSpacing(0);

    QLabel * label = new QLabel(trUtf8("Search"), this);
    m_layout->addWidget(label);
    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_layout->addWidget(m_lineEdit);

    connect(m_focusShortcut, SIGNAL(activated()), this, SLOT(getSelection()));
    connect(m_lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(changeSearchData()));
    connect(m_lineEdit, SIGNAL(returnPressed()), this, SLOT(changeSearchData()));
}

void SearchBar::getSelection()
{
    QString text(m_webView->selectedText());
    if(text.isEmpty())
    {
        text = QApplication::clipboard()->text();
    }
    if(!text.isEmpty())
    {
        m_lineEdit->setText(text);
    }
    m_lineEdit->setFocus();
    m_lineEdit->selectAll();
}

void SearchBar::changeSearchData()
{
    QWebPage::FindFlags flags = QWebPage::FindWrapsAroundDocument | QWebPage::HighlightAllOccurrences;
    m_webView->findText("", flags);
    m_webView->findText(m_lineEdit->text(), flags);
}
