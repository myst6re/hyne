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

#include "PartyEditor.h"
#include "../Data.h"

PartyEditor::PartyEditor(QWidget *parent)
	: PageWidget(parent)
{
}

void PartyEditor::buildWidget()
{
	QGroupBox *partyGBE = new QGroupBox(tr("Équipe"), this);
	QGridLayout *partyGBL = new QGridLayout(partyGBE);
	partyGBL->addWidget(new QLabel(tr("Menus :")), 0, 0);
	partyGBL->addWidget(new QLabel(tr("À l'écran :")), 1, 0);
	QList<QIcon> icons;
	int i, j;
	for(j=0 ; j<11 ; ++j) {
		icons.append(QIcon(QString(":/images/icons/perso%1.png").arg(j)));
	}

	for(i=0 ; i<3 ; ++i)
	{
		QComboBox *comboBox;
		partyE.append(comboBox = new QComboBox(partyGBE));
		comboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
		comboBox->addItem("-", 255);
		for(j=0 ; j<8 ; ++j) {
			comboBox->addItem(icons.at(j), Data::names().at(j), j);
		}
		partyGBL->addWidget(comboBox, 0, i+1);

		partySortE.append(comboBox = new QComboBox(partyGBE));
		comboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
		comboBox->addItem("-", 255);
		for(j=0 ; j<11 ; ++j) {
			comboBox->addItem(icons.at(j), Data::names().at(j), j);
		}
		partyGBL->addWidget(comboBox, 1, i+1);
	}

	dreamE = new QCheckBox(tr("Seule l'équipe principale est visible (rêve avec Laguna)"), partyGBE);
	partyGBL->addWidget(dreamE, 2, 0, 1, 4, Qt::AlignLeft);

	QGroupBox *positionGBE = new QGroupBox(tr("Position"), this);

	presetPosE = new QComboBox(positionGBE);
	presetPosE->addItem(tr("Position prédéfinie"));
	for(i=0 ; i<21 ; ++i) {
		Point p = Data::wmLocation[i];
		presetPosE->addItem(Data::cities().at(p.city), quint32(p.x) | quint32(p.y) << 16);
		presetPosE->setItemData(i + 1, quint32(p.z) | quint32(p.dir) << 16, Qt::UserRole + 1);
	}

	connect(presetPosE, SIGNAL(currentIndexChanged(int)), SLOT(setPosPresetFromIndex(int)));

	QGridLayout *positionGBL = new QGridLayout(positionGBE);
	positionGBL->addWidget(presetPosE, 0, 0);
	positionGBL->addWidget(new QLabel(tr("X :")), 0, 1);
	positionGBL->addWidget(new QLabel(tr("Y :")), 0, 2);
	positionGBL->addWidget(new QLabel(tr("Triangle ID :")), 0, 3);
	positionGBL->addWidget(new QLabel(tr("Direction :")), 0, 4);

	for(i=0 ; i<3 ; ++i) {
		QSpinBox *spinBox;
		positionGBL->addWidget(new QLabel(tr("Membre %1 :").arg(i+1)), i+1, 0);

		xE.append(spinBox = new QSpinBox(positionGBE));
		spinBox->setRange(-32768, 32767);
		positionGBL->addWidget(spinBox, i+1, 1);

		yE.append(spinBox = new QSpinBox(positionGBE));
		spinBox->setRange(-32768, 32767);
		positionGBL->addWidget(spinBox, i+1, 2);

		idE.append(spinBox = new SpinBox16(positionGBE));
		positionGBL->addWidget(spinBox, i+1, 3);

		dirE.append(spinBox = new SpinBox8(positionGBE));
		positionGBL->addWidget(spinBox, i+1, 4);
	}

	moduleE = new QComboBox(positionGBE);
	moduleE->addItem(tr("Terrain"), 0);
	moduleE->addItem(tr("Terrain"), 1);
	moduleE->addItem(tr("Mappemonde"), 2);
	moduleE->addItem(tr("Combat"), 3);

	connect(moduleE, SIGNAL(currentIndexChanged(int)), SLOT(setPosPresetsVisibilityFromModuleIndex(int)));

	mapE = new QComboBox(positionGBE);
	mapE->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
	QStringList mapList = Data::maplist();
	i=0;
	foreach(const QString &mapName, mapList) {
		mapE->addItem(mapName, i++);
	}

	lastMapE = new QComboBox(positionGBE);
	lastMapE->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
	i=0;
	foreach(const QString &mapName, mapList) {
		lastMapE->addItem(mapName, i++);
	}

	positionGBL->addWidget(moduleE, 4, 0);
	positionGBL->addWidget(new QLabel(tr("Terrain courant")), 4, 1, 1, 2);
	positionGBL->addWidget(mapE, 4, 3, 1, 2);
	positionGBL->addWidget(new QLabel(tr("Terrain précédent")), 5, 1, 1, 2);
	positionGBL->addWidget(lastMapE, 5, 3, 1, 2);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(QMargins());
	layout->addWidget(partyGBE);
	layout->addWidget(positionGBE);
	layout->addStretch();
}

void PartyEditor::fillPage()
{
	for(quint8 i=0 ; i<3 ; ++i)
	{
		setCurrentIndex(partyE.at(i), data->misc1.party[i]);
		setCurrentIndex(partySortE.at(i), data->misc2.party[i]);

		xE.at(i)->setValue(data->misc2.x[i]);
		yE.at(i)->setValue(data->misc2.y[i]);
		idE.at(i)->setValue(data->misc2.id[i]);
		dirE.at(i)->setValue(data->misc2.dir[i]);
	}
	dreamE->setChecked(data->misc2.dream & 1);

	int index = moduleE->findData(data->misc2.module);
	if(index != -1) {
		moduleE->setCurrentIndex(index);
	} else {
		moduleE->addItem(tr("Inconnu (%1)").arg(data->misc2.module), data->misc2.module);
	}

	index = mapE->findData(data->misc2.location);
	if(index != -1) {
		mapE->setCurrentIndex(index);
	} else {
		mapE->addItem(tr("Inconnu (%1)").arg(data->misc2.location), data->misc2.location);
	}

	index = lastMapE->findData(data->misc2.location_last);
	if(index != -1) {
		lastMapE->setCurrentIndex(index);
	} else {
		lastMapE->addItem(tr("Inconnu (%1)").arg(data->misc2.location_last), data->misc2.location_last);
	}
}

void PartyEditor::savePage()
{
	for(quint8 i=0 ; i<3 ; ++i)
	{
		data->misc1.party[i] = partyE.at(i)->itemData(partyE.at(i)->currentIndex()).toUInt();
		data->misc2.party[i] = partySortE.at(i)->itemData(partySortE.at(i)->currentIndex()).toUInt();
		data->misc2.x[i] = xE.at(i)->value();
		data->misc2.y[i] = yE.at(i)->value();
		data->misc2.id[i] = idE.at(i)->value();
		data->misc2.dir[i] = dirE.at(i)->value();
	}
	data->misc2.dream = int(dreamE->isChecked()) | (data->misc2.dream & 0xFE);
	data->misc2.module = moduleE->itemData(moduleE->currentIndex()).toInt();
	data->misc2.location = mapE->itemData(mapE->currentIndex()).toInt();
	data->misc2.location_last = lastMapE->itemData(lastMapE->currentIndex()).toInt();

}

void PartyEditor::setPosPresetFromIndex(int index)
{
	if (index == 0) {
		return;
	}

	qint32 data = presetPosE->itemData(index).toInt(),
	        data2 = presetPosE->itemData(index, Qt::UserRole + 1).toInt(),
	        x = data & 0xFFFF, y = (data >> 16) & 0xFFFF,
	        z = data2 & 0xFFFF, dir = (data2 >> 16) & 0xFFFF;

	xE.first()->setValue(x);
	yE.first()->setValue(y);
	idE.first()->setValue(z);
	dirE.first()->setValue(dir);

	for (int i=1 ; i < xE.size(); ++i) {
		xE.at(i)->setValue(0);
		yE.at(i)->setValue(0);
		idE.at(i)->setValue(0);
		dirE.at(i)->setValue(0);
	}

	presetPosE->setCurrentIndex(0);
}

void PartyEditor::setPosPresetsVisibilityFromModuleIndex(int index)
{
	presetPosE->setVisible(index == 2);
}
