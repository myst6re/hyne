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

#ifndef DEF_OBJETEDITOR
#define DEF_OBJETEDITOR

#include "SpinBoxDelegate.h"
#include "PageWidget.h"

class ItemEditor : public PageWidget
{
	Q_OBJECT
public:
	explicit ItemEditor(QWidget *parent=0);
	inline QString name() { return tr("Objets"); }
public slots:
	void fillPage();
	void savePage();
private slots:
	void allItems();
	void sortByType();
	void sortByAlpha();
	void selectAllWeapons(bool);
	void selectAllTimberManiacs(bool select);
protected:
	void buildWidget();
private:
	QWidget *buildPage1();
	QWidget *buildPage2();
	QWidget *buildPage3();

	QTreeView *itemE_view;
	QStandardItemModel *itemE_model;
	QListWidget *battle_itemE_list;
	QTreeWidget *weaponsE_list, *timbermaniacsE_list;
};

#endif
