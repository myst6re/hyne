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

#ifndef DEF_BATTLEEDITOR
#define DEF_BATTLEEDITOR

#include "PageWidget.h"

class BattleEditor : public PageWidget
{
	Q_OBJECT
public:
	explicit BattleEditor(QWidget *parent=0);
	inline QString name() { return tr("Combats"); }
	void updateMode(bool mode);
public slots:
	void fillPage();
	void savePage();

private slots:
	void selectAllDraw(bool selected);
	void selectAllScan(bool selected);
protected:
	void buildWidget();
private:
	QGroupBox *unknownGroupE;
	QDoubleSpinBox *battlewinE, *tombyE, *monsterkillsE, *unknown1E, *unknown2E;
	QSpinBox *battleescE;
	QCheckBox *tombySrE, *firstr1E, *firsteleE, *firstmtlE, *firstirvinelbE;
	QTreeWidget *firstdrawE_list, *firstscanE_list;
};

#endif
