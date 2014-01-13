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

#ifndef TABMANAGER_H
#define TABMANAGER_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

class ITab;
class WebTab;
class RestoreItem;

class TabManager : public QTabWidget
{
    Q_OBJECT

public:
    enum AddTabPolicy
    {
        AddAfterLast,
        AddAfterCurrent
    };

    static TabManager * tabManager();

    ITab * tab(int index) const;
    ITab * currentTab() const;

    QList<RestoreItem *> restoreItems(int maxItems) const;

    AddTabPolicy addTabPolicy() const;
    void setAddTabPolicy(AddTabPolicy policy);

    void insertTab(int index, ITab * tab);
    void addTab(ITab * tab);
    WebTab * insertTab(int index, const QUrl & url);
    WebTab * addChildTab();

    void restoreTab(int index);

public slots:
    WebTab * addTab(const QUrl & url = QUrl());
    void removeTab(int index);
    void removeCurrentTab();

    void clearRestoreList();

    void load();
    void save();

protected:
    TabManager();

private:
    static TabManager * m_tabManager;

    AddTabPolicy m_addTabPolicy;

    QToolButton * m_openTabButton;
    QShortcut * m_closeTabShortcut;

    QList<RestoreItem *> m_restoreList;

    void loadRestoreList();
    void loadTabs();
    void saveRestoreList();
    void saveTabs();

private slots:
    void updateTitle();
    void updateIcon();

signals:
    void restoreListChanged();
};

#endif // TABMANAGER_H
