/****************************************************************************
 ** Hyne Final Fantasy VIII Save Editor
 ** Copyright (C) 2009-2012 Arzel Jérôme <myst6re@gmail.com>
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

#ifndef DEF_SHOPEDITOR
#define DEF_SHOPEDITOR

#include "PageWidget.h"

class ShopEditor : public PageWidget
{
	Q_OBJECT
public:
	explicit ShopEditor(QWidget *parent=0);
public slots:
	void fillPage();
	void savePage();
private slots:
	void setShopEnabled(bool enabled);
protected:
	void buildWidget();
private:
	int getMagType();
	static int items[6][16];
	static QList<int> rareItems[6];

	QListWidget *shopList, *itemList;
	QCheckBox *shopEnabledE;
	SHOP *shops_data;

	QBrush normalItemColor;
};

#endif
