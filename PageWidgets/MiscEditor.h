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

#ifndef DEF_MISCEDITOR
#define DEF_MISCEDITOR

#include "PageWidget.h"
#include "../TimeWidget.h"
#include "../SpinBox.h"

class MiscEditor : public PageWidget
{
	Q_OBJECT
public:
	explicit MiscEditor(QWidget *parent = nullptr);
	inline QString name() { return tr("Divers"); }
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
	// Misc
	TimeWidget *timeE, *countdownE;
	QSpinBox *seedLvlE;
	SpinBox16 *seedExpE, *lastFieldE;
	QLabel *lastFieldLabel, *currentFrameLabel;
	SpinBox32 *argentE, *lagunaGilsE, *currentFrameE;
	SpinBox32 *stepsE;
	SpinBox8 *testSeedE;
	// Unknown
	QGroupBox *unknownE;
	SpinBox16 *unknown1E, *unknown2E, *unknown5E;
	SpinBox32 *unknown4E, *unknown6E, *unknown7E, *unknown8E, *unknown9E;
	// Page 2
	QTreeWidget *tutoE_list;
};

#endif
