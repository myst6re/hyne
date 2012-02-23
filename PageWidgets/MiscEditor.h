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

#ifndef DEF_MISCEDITOR
#define DEF_MISCEDITOR

#include "PageWidget.h"

class MiscEditor : public PageWidget
{
	Q_OBJECT
public:
	explicit MiscEditor(QWidget *parent=0);
	void updateMode(bool mode);
	void updateTime();
public slots:
	void fillPage();
	void savePage();
private slots:
	void selectAll(bool);
	void nivS(int);
	void expS(int);
protected:
	void buildWidget();
private:
	QWidget *buildPage1();
	QWidget *buildPage2();
	// Page 1
	QTabWidget *tabWidget;
	// Party
	QList<QComboBox *> partyE, partySortE;
	QCheckBox *dreamE;
	// Misc
	QSpinBox *tempsSecE, *tempsMinE, *tempsHourE;
	QSpinBox *countdownSecE, *countdownMinE, *countdownHourE;
	QSpinBox *seedLvlE, *seedExpE;
	QLabel *lastFieldLabel, *currentFrameLabel;
	QDoubleSpinBox *argentE, *lagunaGilsE, *currentFrameE;
	QDoubleSpinBox *stepsE;
	QSpinBox *testSeedE, *lastFieldE;
	QLabel *hpMaxLeaderLabel;
	// Header
	QComboBox *locationIDE;
	QSpinBox *hpMaxLeaderE, *saveCountE;
	QDoubleSpinBox *curSaveE;
	// Unknown
	QGroupBox *unknownE;
	QSpinBox *unknown1E, *unknown2E, *unknown5E;
	QDoubleSpinBox *unknown4E, *unknown6E, *unknown7E, *unknown8E, *unknown9E;
	// Page 2
	QTreeWidget *tutoE_list;
};

#endif
