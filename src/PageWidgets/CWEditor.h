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

#include "../FF8Text.h"
#include "PageWidget.h"
#include "../SpinBox.h"

class CWEditor : public PageWidget
{
	Q_OBJECT
public:
	explicit CWEditor(QWidget *parent = nullptr);
	inline QString name() { return tr("Chocobo World"); }
public slots:
	void fillPage();
	void savePage();
private slots:
	void setCWEnabled(bool enabled);
protected:
	void buildWidget();
private:
	QLineEdit *bokoE;
	SpinBox8 *levelE, *currentHpE, *maxHpE;
	QSpinBox *idE, *weaponE;
	SpinBox8 *rankE;
	SpinBox8 *itemClassACountE, *itemClassBCountE, *itemClassCCountE, *itemClassDCountE;
	SpinBox32 *saveCountE;
	QGroupBox *enabledE;
	QList<QCheckBox *> questE;
	QList<QRadioButton *> starCountE;
};
