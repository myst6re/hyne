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

PreviewWidget::PreviewWidget(QWidget *parent) :
	QWidget(parent), _saveData(0)
{
}

void PreviewWidget::paintEvent(QPaintEvent *event)
{
	if (!_saveData)	return;
	QPainter p(this);

	QTransform transform;
	transform.scale(width() * 1.0 / sizeHint().width(), height() * 1.0 / sizeHint().height());
	p.setTransform(transform);

	SavecardView::renderSave(&p, _saveData, 0, event->rect());
}

PreviewEditor::PreviewEditor(QWidget *parent)
	: PageWidget(parent)
{
}

void PreviewEditor::buildWidget()
{
	previewWidget = new PreviewWidget(this);

	locationIDE = new QComboBox(this);
	int i=0;
	for (const QString &loc : Data::locations().list())
		locationIDE->addItem(loc, i++);
	saveCountE = new SpinBox16(this);
	curSaveE = new SpinBox32(this);

	autoGroup = new QGroupBox(tr("Auto."));
	autoGroup->setCheckable(true);

	hpLeaderE = new SpinBox16(autoGroup);
	hpMaxLeaderE = new SpinBox16(autoGroup);
	gilsE = new SpinBox32(autoGroup);
	timeE = new TimeWidget(autoGroup);
	nivLeaderE = new SpinBox8(autoGroup);
	discE = new SpinBox32(autoGroup);
	discE->setRange(1, double(MAX_INT32) + 1.0);

	QList<QIcon> icons;

	for (int i = 0; i < 16; ++i) {
		icons.append(QIcon(QString(":/images/icons/perso%1.png").arg(i)));
	}

	for (int i = 0; i < 3; ++i) {
		QComboBox *comboBox;
		partyE.append(comboBox = new QComboBox(autoGroup));
		comboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
		comboBox->addItem("-", 255);
		for (int j = 0; j < 16; ++j) {
			comboBox->addItem(icons.at(j), Data::names().at(j), j);
		}
	}

	QHBoxLayout *partyL = new QHBoxLayout;
	partyL->addWidget(partyE.first());
	partyL->addWidget(partyE.at(1));
	partyL->addWidget(partyE.last());
	partyL->setContentsMargins(QMargins());

	QGridLayout *autoL = new QGridLayout(autoGroup);
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
	autoL->addWidget(new QLabel(tr("Équipe"), autoGroup), 2, 0);
	autoL->addLayout(partyL, 2, 1, 1, 5);

	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(previewWidget, 0, 0, 1, 6, Qt::AlignCenter);
	layout->addWidget(new QLabel(tr("Lieu affiché")), 1, 0);
	layout->addWidget(locationIDE, 1, 1);
	layout->addWidget(new QLabel(tr("Nombre de sauvegardes")), 1, 2);
	layout->addWidget(saveCountE, 1, 3);
	layout->addWidget(new QLabel(tr("Sauvegarde courante")), 1, 4);
	layout->addWidget(curSaveE, 1, 5);
	layout->addWidget(autoGroup, 2, 0, 1, 6);
	layout->setRowStretch(3, 1);

	connect(autoGroup, SIGNAL(toggled(bool)), SLOT(setGroupDisabled(bool)));
	connect(nivLeaderE, SIGNAL(valueChanged(int)), SLOT(updatePreview()));
	connect(gilsE, SIGNAL(valueChanged(double)), SLOT(updatePreview()));
	connect(timeE, SIGNAL(valueChanged()), SLOT(updatePreview()));
	connect(discE, SIGNAL(valueChanged(double)), SLOT(updatePreview()));
	connect(locationIDE, SIGNAL(currentIndexChanged(int)), SLOT(updatePreview()));
	for (QComboBox *cb : qAsConst(partyE)) {
		connect(cb, SIGNAL(currentIndexChanged(int)), SLOT(updatePreview()));
	}
}

void PreviewEditor::fillPage()
{
	autoGroup->blockSignals(true);
	nivLeaderE->blockSignals(true);
	gilsE->blockSignals(true);
	timeE->blockSignals(true);
	discE->blockSignals(true);
	locationIDE->blockSignals(true);
	for (QComboBox *cb : qAsConst(partyE)) {
		cb->blockSignals(true);
	}

	previewWidget->setSaveData(saveData);
	autoGroup->setChecked(saveData->isPreviewAuto());
	setGroupDisabled(autoGroup->isChecked());
	setCurrentIndex(locationIDE, descData->locationID);
	saveCountE->setValue(descData->saveCount);
	curSaveE->setValue(descData->curSave);
	hpLeaderE->setValue(descData->hpLeader);
	hpMaxLeaderE->setValue(descData->hpMaxLeader);
	gilsE->setValue(descData->gils);
	timeE->setTime(descData->time, saveData->freqValue());
	nivLeaderE->setValue(descData->nivLeader);
	discE->setValue(descData->disc + 1);
	for (int i = 0; i < 3; ++i) {
		setCurrentIndex(partyE.at(i), descData->party[i]);
	}

	autoGroup->blockSignals(false);
	nivLeaderE->blockSignals(false);
	gilsE->blockSignals(false);
	timeE->blockSignals(false);
	discE->blockSignals(false);
	locationIDE->blockSignals(false);
	for (QComboBox *cb : qAsConst(partyE)) {
		cb->blockSignals(false);
	}
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
	descData->time = timeE->time(saveData->freqValue());
	descData->nivLeader = nivLeaderE->value();
	descData->disc = discE->value() - 1;
	for (int i = 0; i < 3; ++i) {
		descData->party[i] = partyE.at(i)->itemData(partyE.at(i)->currentIndex()).toUInt();
	}
}

void PreviewEditor::setGroupDisabled(bool disable)
{
	for (QWidget *o : autoGroup->findChildren<QWidget *>()) {
		o->setDisabled(disable);
	}
}

void PreviewEditor::updatePreview()
{
	savePage();
	previewWidget->update();
}
