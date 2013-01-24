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

#ifndef DEF_WORLDMAPEDITOR
#define DEF_WORLDMAPEDITOR

#include "PageWidget.h"
#include "../MapWidget.h"

class WorldmapEditor : public PageWidget
{
	Q_OBJECT
public:
	explicit WorldmapEditor(QWidget *parent=0);
	inline QString name() { return tr("Mappemonde"); }
public slots:
	void fillPage();
	void savePage();
private slots:
	void fillPositions();
protected:
	void buildWidget();
private:
	QWidget *buildPage1();
	QWidget *buildPage2();
	QWidget *fixGroup;
	QLabel *fixWarnIcon, *fixLabel;
	QRadioButton *fix;
	// Page 1
	QSpinBox *stepsE, *currentCarE;
	QDoubleSpinBox *carStepsE;
	QComboBox *currentMapE;
	QList<QCheckBox *> controlE, koyokE, obelE;
//	QCheckBox *blockLunaticE;
	// Page 2
	QListWidget *vehiclesList;
	QList<QSpinBox *> coord;
};

#endif
