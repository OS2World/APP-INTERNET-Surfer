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

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>

class SettingsManager : public QObject
{
    Q_OBJECT

public:
    static SettingsManager * settingsManager();

    bool isImagesEnabled() const;
    void setImagesEnabled(bool enabled);
    bool isJavascriptEnabled() const;
    void setJavascriptEnabled(bool enabled);
    bool isJavaEnabled() const;
    void setJavaEnabled(bool enabled);
    bool isPluginsEnabled() const;
    void setPluginsEnabled(bool enabled);
    QString sansFontFamily() const;
    void setSansFontFamily(const QString & fontFamily);
    QString serifFontFamily() const;
    void setSerifFontFamily(const QString & fontFamily);
    QString monoFontFamily() const;
    void setMonoFontFamily(const QString & fontFamily);
    int standardFontSize() const;
    void setStandardFontSize(int fontSize);
    int monoFontSize() const;
    void setMonoFontSize(int fontSize);

public slots:
    void load();
    void save();

protected:
    SettingsManager();

private:
    static SettingsManager * m_settingsManager;
};

#endif // SETTINGSMANAGER_H
