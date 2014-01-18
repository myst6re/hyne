/****************************************************************************
 ** Hyne Final Fantasy VIII Save Editor
 ** Copyright (C) 2013 Arzel Jérôme <myst6re@gmail.com>
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#ifndef SELECTSAVESDIALOG_H
#define SELECTSAVESDIALOG_H

#include "QtWidgets.h"
#include "SaveData.h"
#include "HelpWidget.h"

class SelectSavesDialog : public QDialog
{
    Q_OBJECT
public:
	SelectSavesDialog(const QList<SaveData *> &saveFiles, bool multiSelection, bool onlyFF8, QWidget *parent=0);
	explicit SelectSavesDialog(const QList<SaveData *> &saveFiles, QWidget *parent=0);
	QList<int> selectedSaves() const;
	QList<int> order() const;
signals:

private slots:
	void controlSelection();
private:
	void fillList(const QList<SaveData *> &saveFiles);
	static QString infoText(bool warn=false);

	QListWidget *list;
	HelpWidget *msg;
};

#endif // SELECTSAVESDIALOG_H
