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

#ifndef DEF_DRAWPOINTEDITOR
#define DEF_DRAWPOINTEDITOR

#include "SpinBoxDelegate.h"
#include "PageWidget.h"

#define DRAWPOINT_COLOR_FILLED			QColor(0xa1,0x62,0xb2)
#define DRAWPOINT_COLOR_HALFFILLED		QColor(0xc7,0x83,0xbf)
#define DRAWPOINT_COLOR_EMPTY			QColor(0x8b,0x73,0xaf)
#define DRAWPOINT_COLOR_CEMPTY			QColor(0x62,0x5a,0xc5)

class DrawPointEditor : public PageWidget
{
	Q_OBJECT
public:
	explicit DrawPointEditor(QWidget *parent=0);
	inline QString name() { return tr("Sources"); }
public slots:
	void fillPage();
	void savePage();
private slots:
	void setItemColor(QStandardItem *item);
	void all();
protected:
	void buildWidget();
private:
	QStandardItemModel *drawE_model;
	QComboBox *listAll;
};

#endif
