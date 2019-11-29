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

#ifndef DEF_PARTYEDITOR
#define DEF_PARTYEDITOR

#include "PageWidget.h"
#include "../SpinBox.h"

class PartyEditor : public PageWidget
{
	Q_OBJECT
public:
	explicit PartyEditor(QWidget *parent=0);
	inline QString name() { return tr("Équipe"); }
public slots:
	void fillPage();
	void savePage();
	void setPosPresetFromIndex(int index);
	void setPosPresetsVisibilityFromModuleIndex(int index);
protected:
	void buildWidget();
private:
	QList<QComboBox *> partyE, partySortE;
	QCheckBox *dreamE;
	QList<QSpinBox *> xE, yE, idE, dirE;
	QComboBox *moduleE, *mapE, *lastMapE, *presetPosE;
};

#endif // DEF_PARTYEDITOR
