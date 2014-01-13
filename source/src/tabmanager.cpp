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

#include "tabmanager.h"
#include "global.h"
#include "itab.h"
#include "webtab.h"
#include "restoreitem.h"

TabManager * TabManager::m_tabManager = 0;

TabManager * TabManager::tabManager()
{
    if(!m_tabManager)
    {
        m_tabManager = new TabManager();
    }
    return m_tabManager;
}

ITab * TabManager::tab(int index) const
{
    return qobject_cast<ITab *>(widget(index));
}

ITab * TabManager::currentTab() const
{
    return tab(currentIndex());
}

QList<RestoreItem *> TabManager::restoreItems(int maxItems) const
{
    return m_restoreList.mid(0, maxItems);
}

TabManager::AddTabPolicy TabManager::addTabPolicy() const
{
    return m_addTabPolicy;
}

void TabManager::setAddTabPolicy(AddTabPolicy policy)
{
    m_addTabPolicy = policy;
}

void TabManager::insertTab(int index, ITab * tab)
{
    int oldIndex = indexOf(tab);
    if(oldIndex == -1)
    {
        QTabWidget::insertTab(index, tab, Global::notEmpty(tab->icon()), "");
        setTabToolTip(index, Global::notEmpty(tab->title()));
        setCurrentIndex(index);
        connect(tab, SIGNAL(titleChanged(const QString &)), this, SLOT(updateTitle()));
        connect(tab, SIGNAL(iconChanged()), this, SLOT(updateIcon()));
    }
    else
    {
        setCurrentIndex(oldIndex);
    }
}

void TabManager::addTab(ITab * tab)
{
    insertTab(count(), tab);
}

WebTab * TabManager::insertTab(int index, const QUrl & url)
{
    WebTab * tab = new WebTab(url, this);
    insertTab(index, tab);
    return tab;
}

WebTab * TabManager::addChildTab()
{
    int index;
    if(addTabPolicy() == AddAfterLast)
    {
        index = count();
    }
    else
    {
        index = currentIndex() + 1;
    }
    return insertTab(index, QUrl());
}

void TabManager::restoreTab(int index)
{
    if(index >= 0 && index < m_restoreList.size())
    {
        addTab(m_restoreList.at(index)->url());
        delete m_restoreList.at(index);
        m_restoreList.removeAt(index);
        emit restoreListChanged();
    }
}

WebTab * TabManager::addTab(const QUrl & url)
{
    return insertTab(count(), url);
}

void TabManager::removeTab(int index)
{
    WebTab * tab = qobject_cast<WebTab *>(this->tab(index));
    if(tab)
    {
        if(!tab->url().isEmpty())
        {
            RestoreItem * item = new RestoreItem(tab->url(), tab->title());
            m_restoreList.prepend(item);
            emit restoreListChanged();
        }
        tab->deleteLater();
    }
    QTabWidget::removeTab(index);
    if(!count())
    {
        addTab();
    }
}

void TabManager::removeCurrentTab()
{
    removeTab(currentIndex());
}

void TabManager::clearRestoreList()
{
    foreach(RestoreItem * tab, m_restoreList)
    {
        delete tab;
    }
    m_restoreList.clear();
    emit restoreListChanged();
}

void TabManager::load()
{
    loadRestoreList();
    loadTabs();
    QSettings settings;
    setCurrentIndex(settings.value("TabManager/currentIndex", 0).toInt());
    setAddTabPolicy(AddTabPolicy(settings.value("TabManager/addTabPolicy", 0).toInt()));
    if(!count())
    {
        addTab();
    }
}

void TabManager::save()
{
    saveRestoreList();
    saveTabs();
    QSettings settings;
    settings.setValue("TabManager/currentIndex", currentIndex());
    settings.setValue("TabManager/addTabPolicy", addTabPolicy());
}

TabManager::TabManager()
{
    tabBar()->setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);

    setDocumentMode(true);
    setMovable(true);
    setTabsClosable(true);

    m_openTabButton = new QToolButton(this);
    m_openTabButton->setAutoRaise(true);
    m_openTabButton->setIcon(QIcon::fromTheme("tab-new", QIcon(":/tab-new.png")));
    m_openTabButton->setToolTip(trUtf8("Open new tab"));
    m_openTabButton->setShortcut(Qt::CTRL + Qt::Key_T);
    setCornerWidget(m_openTabButton);

    m_closeTabShortcut = new QShortcut(QKeySequence::Close, this);

    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(removeTab(int)));
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(updateTitle()));
    connect(m_openTabButton, SIGNAL(clicked()), this, SLOT(addTab()));
    connect(m_closeTabShortcut, SIGNAL(activated()), this, SLOT(removeCurrentTab()));
}

void TabManager::loadRestoreList()
{
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/tabmanager";
    QFile file(path + "/restorelist.xml");
    if(!QDir().mkpath(path) || !file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QXmlStreamReader xml(&file);
    while(!xml.atEnd())
    {
        xml.readNext();
        if(xml.isStartElement() && xml.name() == "surfer")
        {
            while(!xml.atEnd())
            {
                xml.readNext();
                if(xml.isEndElement())
                {
                    break;
                }
                if(xml.isStartElement())
                {
                    if(xml.name() == "item")
                    {
                        QUrl url(xml.attributes().value("href").toString());
                        QString title;
                        while(!xml.atEnd())
                        {
                            xml.readNext();
                            if(xml.isEndElement())
                            {
                                break;
                            }
                            if(xml.isStartElement())
                            {
                                if(xml.name() == "title")
                                {
                                    title = xml.readElementText();
                                }
                                else
                                {
                                    xml.skipCurrentElement();
                                }
                            }
                        }
                        RestoreItem * item = new RestoreItem(url, title);
                        m_restoreList.prepend(item);
                        emit restoreListChanged();
                    }
                    else
                    {
                        xml.skipCurrentElement();
                    }
                }
            }
        }
    }
    file.close();
}

void TabManager::loadTabs()
{
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/tabmanager";
    QFile file(path + "/tabs.xml");
    if(!QDir().mkpath(path) || !file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QXmlStreamReader xml(&file);
    while(!xml.atEnd())
    {
        xml.readNext();
        if(xml.isStartElement() && xml.name() == "surfer")
        {
            while(!xml.atEnd())
            {
                xml.readNext();
                if(xml.isEndElement())
                {
                    break;
                }
                if(xml.isStartElement())
                {
                    if(xml.name() == "item")
                    {
                        QUrl url(xml.attributes().value("href").toString());
                        while(!xml.atEnd())
                        {
                            xml.readNext();
                            if(xml.isEndElement())
                            {
                                break;
                            }
                            if(xml.isStartElement())
                            {
                                xml.skipCurrentElement();
                            }
                        }
                        addTab(url);
                    }
                    else
                    {
                        xml.skipCurrentElement();
                    }
                }
            }
        }
    }
    file.close();
}

void TabManager::saveRestoreList()
{
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/tabmanager";
    QFile file(path + "/restorelist.xml");
    if(!QDir().mkpath(path) || !file.open(QIODevice::WriteOnly))
    {
        return;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("surfer");
    for(int i = m_restoreList.size() - 1; i >= 0; i--)
    {
        RestoreItem * item = m_restoreList.at(i);
        xml.writeStartElement("item");
        xml.writeAttribute("href", item->url().toString());
        xml.writeTextElement("title", item->title());
        xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();
}

void TabManager::saveTabs()
{
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/tabmanager";
    QFile file(path + "/tabs.xml");
    if(!QDir().mkpath(path) || !file.open(QIODevice::WriteOnly))
    {
        return;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("surfer");
    for(int i = 0; i < count(); i++)
    {
        WebTab * tab = qobject_cast<WebTab *>(this->tab(i));
        if(tab)
        {
            xml.writeStartElement("item");
            xml.writeAttribute("href", tab->url().toString());
            xml.writeEndElement();
        }
        else
        {
            removeTab(i);
            i--;
        }
    }
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();
}

void TabManager::updateTitle()
{
    ITab * tab = qobject_cast<ITab *>(sender());
    if(!tab)
    {
        tab = currentTab();
    }
    if(tab == currentTab())
    {
        if(tab->title().isEmpty())
        {
            window()->setWindowTitle("Surfer");
        }
        else
        {
            window()->setWindowTitle(tab->title() + " - Surfer");
        }
    }
    setTabToolTip(indexOf(tab), Global::notEmpty(tab->title()));
}

void TabManager::updateIcon()
{
    ITab * tab = qobject_cast<ITab *>(sender());
    setTabIcon(indexOf(tab), Global::notEmpty(tab->icon()));
}
