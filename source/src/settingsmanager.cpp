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

#include "settingsmanager.h"

SettingsManager * SettingsManager::m_settingsManager = 0;

SettingsManager::SettingsManager()
{
}

bool SettingsManager::isImagesEnabled() const
{
    return QWebSettings::globalSettings()->testAttribute(QWebSettings::AutoLoadImages);
}

void SettingsManager::setImagesEnabled(bool enabled)
{
    QWebSettings::globalSettings()->setAttribute(QWebSettings::AutoLoadImages, enabled);
}

bool SettingsManager::isJavascriptEnabled() const
{
    return QWebSettings::globalSettings()->testAttribute(QWebSettings::JavascriptEnabled);
}

void SettingsManager::setJavascriptEnabled(bool enabled)
{
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptEnabled, enabled);
}

bool SettingsManager::isJavaEnabled() const
{
    return QWebSettings::globalSettings()->testAttribute(QWebSettings::JavaEnabled);
}

void SettingsManager::setJavaEnabled(bool enabled)
{
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavaEnabled, enabled);
}

bool SettingsManager::isPluginsEnabled() const
{
    return QWebSettings::globalSettings()->testAttribute(QWebSettings::PluginsEnabled);
}

void SettingsManager::setPluginsEnabled(bool enabled)
{
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, enabled);
}

QString SettingsManager::sansFontFamily() const
{
    return QWebSettings::globalSettings()->fontFamily(QWebSettings::StandardFont);
}

void SettingsManager::setSansFontFamily(const QString & fontFamily)
{
    QWebSettings::globalSettings()->setFontFamily(QWebSettings::StandardFont, fontFamily);
}

QString SettingsManager::serifFontFamily() const
{
    return QWebSettings::globalSettings()->fontFamily(QWebSettings::SerifFont);
}

void SettingsManager::setSerifFontFamily(const QString & fontFamily)
{
    QWebSettings::globalSettings()->setFontFamily(QWebSettings::SerifFont, fontFamily);
}

QString SettingsManager::monoFontFamily() const
{
    return QWebSettings::globalSettings()->fontFamily(QWebSettings::FixedFont);
}

void SettingsManager::setMonoFontFamily(const QString & fontFamily)
{
    QWebSettings::globalSettings()->setFontFamily(QWebSettings::FixedFont, fontFamily);
}

int SettingsManager::standardFontSize() const
{
    return QWebSettings::globalSettings()->fontSize(QWebSettings::DefaultFontSize);
}

void SettingsManager::setStandardFontSize(int fontSize)
{
    QWebSettings::globalSettings()->setFontSize(QWebSettings::DefaultFontSize, fontSize);
}

int SettingsManager::monoFontSize() const
{
    return QWebSettings::globalSettings()->fontSize(QWebSettings::DefaultFixedFontSize);
}

void SettingsManager::setMonoFontSize(int fontSize)
{
    QWebSettings::globalSettings()->setFontSize(QWebSettings::DefaultFixedFontSize, fontSize);
}

void SettingsManager::load()
{
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    if(!QDir().mkpath(path))
    {
        return;
    }
    QWebSettings::setIconDatabasePath(path);
    QSettings settings;
    setImagesEnabled(settings.value("WebKit/images", true).toBool());
    setJavascriptEnabled(settings.value("WebKit/javascript", true).toBool());
    setJavaEnabled(settings.value("WebKit/java", true).toBool());
    setPluginsEnabled(settings.value("WebKit/plugins", true).toBool());
    setSansFontFamily(settings.value("WebKit/sansFontFamily").toString());
    setSerifFontFamily(settings.value("WebKit/serifFontFamily").toString());
    setMonoFontFamily(settings.value("WebKit/monoFontFamily").toString());
    setStandardFontSize(settings.value("WebKit/standardFontSize", 12).toInt());
    setMonoFontSize(settings.value("WebKit/monoFontSize", 12).toInt());
}

void SettingsManager::save()
{
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    if(!QDir().mkpath(path))
    {
        return;
    }
    QSettings settings;
    settings.setValue("WebKit/images", isImagesEnabled());
    settings.setValue("WebKit/javascript", isJavascriptEnabled());
    settings.setValue("WebKit/java", isJavaEnabled());
    settings.setValue("WebKit/plugins", isPluginsEnabled());
    settings.setValue("WebKit/sansFontFamily", sansFontFamily());
    settings.setValue("WebKit/serifFontFamily", serifFontFamily());
    settings.setValue("WebKit/monoFontFamily", monoFontFamily());
    settings.setValue("WebKit/standardFontSize", standardFontSize());
    settings.setValue("WebKit/monoFontSize", monoFontSize());
}

SettingsManager * SettingsManager::settingsManager()
{
    if(!m_settingsManager)
    {
        m_settingsManager = new SettingsManager();
    }
    return m_settingsManager;
}
