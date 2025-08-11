/****************************************************************************
 ** Hyne Final Fantasy VIII Save Editor
 ** Copyright (C) 2009-2013 Arzel Jérôme <myst6re@gmail.com>
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

#pragma once

#include <QtWidgets>
#include "SaveData.h"

class HeaderDialog : public QDialog
{
	Q_OBJECT
public:
	enum ViewType { NormalView, RestoreView, CreateView };
	HeaderDialog(SaveData *saveData, QWidget *parent = nullptr, ViewType viewType = NormalView);
	void setCountry(char c);
	void setCode(const QString &codestr);
	void setId(const QString &idStr);
public slots:
	virtual void accept();
private slots:
	inline void saveIcon1() { saveIcon(); }
	inline void saveIcon2() { saveIcon(true); }
private:
	static void fillCode(QComboBox *code);
	static void fillId(QComboBox *id, int saveId);

	void fill();
	void saveIcon(bool chocobo_world_icon = false);

	QGroupBox *group1;
	QLabel *exists_lbl;
	QCheckBox *exists;
	QComboBox *country;
	QComboBox *code;
	QComboBox *id;
	QGroupBox *group2;
	QPushButton *icon2_saveButton;
	QComboBox *icon1, *icon2;
	QLineEdit *desc;
	QCheckBox *desc_auto;
	QLabel *id_lbl, *bloc, *icon2_lbl;
	SaveData *saveData;
	ViewType viewType;
};
