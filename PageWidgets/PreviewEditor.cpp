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

#include "PreviewEditor.h"
#include "Parameters.h"

PreviewEditor::PreviewEditor(QWidget *parent)
	: PageWidget(parent)
{
}

void PreviewEditor::buildWidget()
{
	previewWidget = new PreviewWidget(this);

	locationIDE = new QComboBox(this);
	int i=0;
	foreach(const QString &loc, Data::locations().list())
		locationIDE->addItem(loc, i++);
	saveCountE = new QSpinBox(this);
	saveCountE->setRange(0, MAX_INT16);
	curSaveE = new QDoubleSpinBox(this);
	curSaveE->setRange(0, MAX_INT32);
	curSaveE->setDecimals(0);

	autoGroup = new QGroupBox(tr("Auto."));
	autoGroup->setCheckable(true);

	hpLeaderE = new QSpinBox(autoGroup);
	hpLeaderE->setRange(0, MAX_INT16);
	hpMaxLeaderE = new QSpinBox(autoGroup);
	hpMaxLeaderE->setRange(0, MAX_INT16);
	gilsE = new QDoubleSpinBox(autoGroup);
	gilsE->setRange(0, MAX_INT32);
	gilsE->setDecimals(0);
	timeE = new QDoubleSpinBox(autoGroup);
	timeE->setRange(0, MAX_INT32);
	timeE->setDecimals(0);
	nivLeaderE = new QSpinBox(autoGroup);
	nivLeaderE->setRange(0, MAX_INT8);
	//partyE = new QSpinBox(autoGroup);
	discE = new QDoubleSpinBox(autoGroup);
	discE->setRange(1, MAX_INT32 + 1);
	discE->setDecimals(0);

	autoL = new QGridLayout(autoGroup);
	autoL->addWidget(new QLabel(tr("HP leader (inutilisé)"), autoGroup), 0, 0);
	autoL->addWidget(hpLeaderE, 0, 1);
	autoL->addWidget(new QLabel(tr("HP max. leader (inutilisé)"), autoGroup), 0, 2);
	autoL->addWidget(hpMaxLeaderE, 0, 3);
	autoL->addWidget(new QLabel(tr("Niveau leader"), autoGroup), 0, 4);
	autoL->addWidget(nivLeaderE, 0, 5);
	autoL->addWidget(new QLabel(tr("Argent"), autoGroup), 1, 0);
	autoL->addWidget(gilsE, 1, 1);
	autoL->addWidget(new QLabel(tr("Temps"), autoGroup), 1, 2);
	autoL->addWidget(timeE, 1, 3);
	autoL->addWidget(new QLabel(tr("Disque"), autoGroup), 1, 4);
	autoL->addWidget(discE, 1, 5);

	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(previewWidget, 0, 0, 1, 6, Qt::AlignCenter);
	layout->addWidget(new QLabel(tr("Lieu")), 1, 0);
	layout->addWidget(locationIDE, 1, 1);
	layout->addWidget(new QLabel(tr("Nombre de sauvegardes")), 1, 2);
	layout->addWidget(saveCountE, 1, 3);
	layout->addWidget(new QLabel(tr("Sauvegarde courante")), 1, 4);
	layout->addWidget(curSaveE, 1, 5);
	layout->addWidget(autoGroup, 2, 0, 1, 6);
	layout->setRowStretch(3, 1);

	connect(autoGroup, SIGNAL(toggled(bool)), SLOT(setGroupDisabled(bool)));
	connect(nivLeaderE, SIGNAL(valueChanged(double)), SLOT(updatePreview()));
	connect(gilsE, SIGNAL(valueChanged(double)), SLOT(updatePreview()));
	connect(timeE, SIGNAL(valueChanged(double)), SLOT(updatePreview()));
	connect(discE, SIGNAL(valueChanged(double)), SLOT(updatePreview()));
	connect(locationIDE, SIGNAL(currentIndexChanged(int)), SLOT(updatePreview()));
}

void PreviewEditor::fillPage()
{
	previewWidget->setSaveData(saveData);
	autoGroup->setChecked(saveData->isPreviewAuto());
	setGroupDisabled(autoGroup->isChecked());
	setCurrentIndex(locationIDE, descData->locationID);
	saveCountE->setValue(descData->saveCount);
	curSaveE->setValue(descData->curSave);
	hpLeaderE->setValue(descData->hpLeader);
	hpMaxLeaderE->setValue(descData->hpMaxLeader);
	gilsE->setValue(descData->gils);
	timeE->setValue(descData->time);
	nivLeaderE->setValue(descData->nivLeader);
	discE->setValue(descData->disc + 1);
}

void PreviewEditor::savePage()
{
	saveData->setPreviewAuto(autoGroup->isChecked());
	descData->locationID = locationIDE->itemData(locationIDE->currentIndex()).toUInt();
	descData->saveCount = saveCountE->value();
	descData->curSave = curSaveE->value();
	descData->hpLeader = hpLeaderE->value();
	descData->hpMaxLeader = hpMaxLeaderE->value();
	descData->gils = gilsE->value();
	descData->time = timeE->value();
	descData->nivLeader = nivLeaderE->value();
	descData->disc = discE->value() - 1;
}

void PreviewEditor::setGroupDisabled(bool disable)
{
	foreach(QWidget *o, autoGroup->findChildren<QWidget *>()) {
		o->setDisabled(disable);
	}
}

void PreviewEditor::updatePreview()
{
	savePage();
	previewWidget->update();
}
