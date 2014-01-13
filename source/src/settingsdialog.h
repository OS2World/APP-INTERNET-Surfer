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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtCore>
#include <QtGui>
#include <QtWebKit>
#include "ui_settingsdialog.h"

class SettingsDialog : public QDialog, private Ui::SettingsDialog
{
    Q_OBJECT

public:
    SettingsDialog(QWidget * parent = 0);

public slots:
    void defaults();
    void save();

protected:
    void closeEvent(QCloseEvent * event);

private:
    void load();

private slots:
    void openNewDirectory();
};

#endif // SETTINGSDIALOG_H
