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

#ifndef DEF_EDITOR
#define DEF_EDITOR

#include <QtWidgets>
#include "SaveData.h"

class Editor : public QWidget
{
	Q_OBJECT
public:
	explicit Editor(QWidget *parent = nullptr);
	void load(SaveData *saveData, bool pc);
	void updateMode(bool mode);
	void updateTime();
private:
	QStackedLayout *stackedLayout;
	QListWidget *liste;
	SaveData *saveData;
	SaveData saveDataCopy;
	bool pc;
signals:
	void accepted();
	void rejected();
public slots:
	void save();
private slots:
	void setCurrentSection(QListWidgetItem *current, QListWidgetItem *previous = nullptr);
};

#endif
