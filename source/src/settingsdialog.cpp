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

#include "settingsdialog.h"
#include "settingsmanager.h"
#include "historymanager.h"
#include "downloadmanager.h"
#include "networkaccessmanager.h"
#include "tabmanager.h"

SettingsDialog::SettingsDialog(QWidget * parent)
    : QDialog(parent)
{
    setupUi(this);

    QSettings settings;
    QRect geometry = settings.value("SettingsDialog/geometry").toRect();
    if(!geometry.isEmpty())
    {
        setGeometry(geometry);
    }

    m_defaultsButton->setIcon(QIcon::fromTheme("document-revert", QIcon(":/document-revert.png")));
    m_okButton->setIcon(QIcon::fromTheme("dialog-ok", QIcon(":/dialog-ok.png")));
    m_applyButton->setIcon(QIcon::fromTheme("dialog-ok-apply", QIcon(":/dialog-ok-apply.png")));
    m_cancelButton->setIcon(QIcon::fromTheme("dialog-cancel", QIcon(":/dialog-cancel.png")));

    m_historyExpirationComboBox->setItemData(0, 1);
    m_historyExpirationComboBox->setItemData(1, 3);
    m_historyExpirationComboBox->setItemData(2, 7);
    m_historyExpirationComboBox->setItemData(3, 14);
    m_historyExpirationComboBox->setItemData(4, 30);
    m_historyExpirationComboBox->setItemData(5, 90);
    m_historyExpirationComboBox->setItemData(6, 180);
    m_historyExpirationComboBox->setItemData(7, 360);

    connect(m_defaultsButton, SIGNAL(clicked()), this, SLOT(defaults()));
    connect(m_okButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(m_okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_applyButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(m_downloadCheckBox, SIGNAL(toggled(bool)), m_downloadToolButton, SLOT(setEnabled(bool)));
    connect(m_downloadToolButton, SIGNAL(clicked()), this, SLOT(openNewDirectory()));
    connect(m_listWidget, SIGNAL(currentRowChanged(int)), m_stackedWidget, SLOT(setCurrentIndex(int)));

    m_listWidget->setCurrentRow(settings.value("SettingsDialog/row").toInt());

    load();
}

void SettingsDialog::defaults()
{
    m_imagesCheckBox->setChecked(true);
    m_javascriptCheckBox->setChecked(true);
    m_javaCheckBox->setChecked(true);
    m_pluginsCheckBox->setChecked(true);
    m_standardFontSpinBox->setValue(12);
    m_monoFontSpinBox->setValue(12);
    m_privateBrowsingCheckBox->setChecked(false);
    m_historyExpirationComboBox->setCurrentIndex(2);
    m_downloadCheckBox->setChecked(false);
    m_cacheSizeSpinBox->setValue(50);
    m_proxyLineEdit->setText("");
    m_proxySpinBox->setValue(0);
    m_addTabPolicyComboBox->setCurrentIndex(0);
}

void SettingsDialog::save()
{
    SettingsManager * manager = SettingsManager::settingsManager();
    HistoryManager * historyManager = HistoryManager::historyManager();
    DownloadManager * downloadManager = DownloadManager::downloadManager();
    NetworkAccessManager * networkAccessManager = NetworkAccessManager::networkAccessManager();
    TabManager * tabManager = TabManager::tabManager();
    manager->setImagesEnabled(m_imagesCheckBox->isChecked());
    manager->setJavascriptEnabled(m_javascriptCheckBox->isChecked());
    manager->setJavaEnabled(m_javaCheckBox->isChecked());
    manager->setPluginsEnabled(m_pluginsCheckBox->isChecked());
    manager->setSansFontFamily(m_sansFontComboBox->currentFont().family());
    manager->setSerifFontFamily(m_serifFontComboBox->currentFont().family());
    manager->setMonoFontFamily(m_monoFontComboBox->currentFont().family());
    manager->setStandardFontSize(m_standardFontSpinBox->value());
    manager->setMonoFontSize(m_monoFontSpinBox->value());
    historyManager->setPrivateBrowsingEnabled(m_privateBrowsingCheckBox->isChecked());
    int index = m_historyExpirationComboBox->currentIndex();
    int days = m_historyExpirationComboBox->itemData(index).toInt();
    historyManager->setExpirationDays(days);
    QString path;
    if(m_downloadCheckBox->isChecked())
    {
        path = m_downloadLineEdit->text();
    }
    else
    {
        path = "";
    }
    downloadManager->setStandardPath(path);
    networkAccessManager->setCacheSize(m_cacheSizeSpinBox->value() * 1024 * 1024);
    QUrl url;
    if(!m_proxyLineEdit->text().isEmpty())
    {
        url.setHost(m_proxyLineEdit->text());
        url.setPort(m_proxySpinBox->value());
    }
    networkAccessManager->setProxy(url);
    tabManager->setAddTabPolicy(TabManager::AddTabPolicy(m_addTabPolicyComboBox->currentIndex()));
}

void SettingsDialog::closeEvent(QCloseEvent * event)
{
    QSettings settings;
    settings.setValue("SettingsDialog/geometry", geometry());
    settings.setValue("SettingsDialog/row", m_listWidget->currentRow());

    event->accept();
}

void SettingsDialog::load()
{
    SettingsManager * manager = SettingsManager::settingsManager();
    HistoryManager * historyManager = HistoryManager::historyManager();
    DownloadManager * downloadManager = DownloadManager::downloadManager();
    NetworkAccessManager * networkAccessManager = NetworkAccessManager::networkAccessManager();
    TabManager * tabManager = TabManager::tabManager();
    m_imagesCheckBox->setChecked(manager->isImagesEnabled());
    m_javascriptCheckBox->setChecked(manager->isJavascriptEnabled());
    m_javaCheckBox->setChecked(manager->isJavaEnabled());
    m_pluginsCheckBox->setChecked(manager->isPluginsEnabled());
    m_sansFontComboBox->setCurrentFont(QFont(manager->sansFontFamily()));
    m_serifFontComboBox->setCurrentFont(QFont(manager->serifFontFamily()));
    m_monoFontComboBox->setCurrentFont(QFont(manager->monoFontFamily()));
    m_standardFontSpinBox->setValue(manager->standardFontSize());
    m_monoFontSpinBox->setValue(manager->monoFontSize());
    m_privateBrowsingCheckBox->setChecked(historyManager->isPrivateBrowsingEnabled());
    m_historyExpirationComboBox->setCurrentIndex(2);
    for(int i = 0; i < 8; i++)
    {
        if(m_historyExpirationComboBox->itemData(i).toInt() ==
           historyManager->expirationDays())
        {
            m_historyExpirationComboBox->setCurrentIndex(i);
            break;
        }
    }
    QString path = downloadManager->standardPath();
    if(path.isEmpty())
    {
        m_downloadCheckBox->setChecked(false);
    }
    else
    {
        m_downloadCheckBox->setChecked(true);
        m_downloadLineEdit->setText(path);
    }
    m_cacheSizeSpinBox->setValue(networkAccessManager->cacheSize() / 1024 / 1024);
    QUrl url(networkAccessManager->proxy());
    if(!url.isEmpty())
    {
        m_proxyLineEdit->setText(url.host());
        m_proxySpinBox->setValue(url.port());
    }
    m_addTabPolicyComboBox->setCurrentIndex(tabManager->addTabPolicy() == TabManager::AddAfterLast ? 0 : 1);
}

void SettingsDialog::openNewDirectory()
{
    QString path = m_downloadLineEdit->text();
    if(path.isEmpty())
    {
        path = QDir::homePath();
    }
    path = QFileDialog::getExistingDirectory(this, trUtf8("Choose directory"), path);
    if(!path.isEmpty())
    {
        m_downloadLineEdit->setText(path);
    }
}
