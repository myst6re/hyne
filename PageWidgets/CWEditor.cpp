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

#include "CWEditor.h"

CWEditor::CWEditor(QWidget *parent)
	: PageWidget(parent)
{
}

void CWEditor::buildWidget()
{
	bokoE = new QLineEdit(this);

	QLabel *bokoPix = new QLabel(this);
	bokoPix->setPixmap(QPixmap(":/images/icons/perso14.png"));

	QLabel *mogPix = new QLabel(this);
	mogPix->setPixmap(QPixmap(":/images/icons/perso13.png"));

	QVBoxLayout *pixLayout = new QVBoxLayout;
	pixLayout->addWidget(bokoPix);
	pixLayout->addWidget(mogPix);
	pixLayout->addStretch();
	pixLayout->setContentsMargins(QMargins());

	enabledE = new QGroupBox(tr("Activé"), this);
	enabledE->setCheckable(true);

	levelE = new SpinBox8(enabledE);
	currentHpE = new SpinBox8(enabledE);
	maxHpE = new SpinBox8(enabledE);
	idE = new QSpinBox(enabledE);
	idE->setRange(000, 999);
	weaponE = new QSpinBox(enabledE);
	weaponE->setRange(0000, 9999);
	rankE = new SpinBox8(enabledE);
	rankE->setRange(1, MAX_INT8 + 1);
	saveCountE = new SpinBox32(enabledE);
	itemClassACountE = new SpinBox8(enabledE);
	itemClassBCountE = new SpinBox8(enabledE);
	itemClassCCountE = new SpinBox8(enabledE);
	itemClassDCountE = new SpinBox8(enabledE);

	QGroupBox *questGroupE = new QGroupBox(tr("Avancement quête"), enabledE);
	QGridLayout *questL = new QGridLayout(questGroupE);

	QStringList texts;
	texts << tr("Dans le Chocobo World")
		  << tr("MiniMog trouvé") << tr("MiniMog obtenu")
		  << tr("MiniMog en attente") << tr("Roi démon vaincu")
		  << tr("Événement courant vu") << tr("Event wait OFF");
	for (int i = 0; i < 7; ++i) {
		QCheckBox *questCheckBox = new QCheckBox(texts.at(i));
		questE.append(questCheckBox);
		if (i >= 1) {
			questL->addWidget(questCheckBox, (i - 1) / 3, (i - 1) % 3);
		}
	}

	QGroupBox *starCountGroupE = new QGroupBox(tr("Niveau d'invocation en jeu"), enabledE);
	QGridLayout *starCountL = new QGridLayout(starCountGroupE);

	texts.clear();
	texts << tr("Chocobraise") << tr("Chocoflammes")
		  << tr("Chocométéore") << tr("Grochocobo");
	for (int i = 0; i < 4; ++i) {
		QRadioButton *starCountRadio = new QRadioButton(texts.at(i));
		starCountE.append(starCountRadio);
		starCountL->addWidget(starCountRadio, i / 2, i % 2);
	}

	QGridLayout *grid = new QGridLayout(enabledE);
	grid->addWidget(questE.at(0), 0, 0, 1, 2);
	grid->addWidget(new QLabel(tr("ID :"),enabledE), 0, 2);
	grid->addWidget(idE, 0, 3);
	grid->addWidget(new QLabel(tr("Niveau :"),enabledE), 1, 0);
	grid->addWidget(levelE, 1, 1);
	grid->addWidget(new QLabel(tr("Arme :"),enabledE), 1, 2);
	grid->addWidget(weaponE, 1, 3);
	grid->addWidget(new QLabel(tr("HP actuels :"),enabledE), 2, 0);
	grid->addWidget(currentHpE, 2, 1);
	grid->addWidget(new QLabel(tr("HP max :"),enabledE), 2, 2);
	grid->addWidget(maxHpE, 2, 3);
	grid->addWidget(new QLabel(tr("Rang (1 est le meilleur) :"),enabledE), 3, 0);
	grid->addWidget(rankE, 3, 1);
	grid->addWidget(new QLabel(tr("Nombre de sauvegardes :"),enabledE), 3, 2);
	grid->addWidget(saveCountE, 3, 3);
	grid->addWidget(new QLabel(tr("Nombre d'objets de classe A :"),enabledE), 4, 0);
	grid->addWidget(itemClassACountE, 4, 1);
	grid->addWidget(new QLabel(tr("Nombre d'objets de classe B :"),enabledE), 4, 2);
	grid->addWidget(itemClassBCountE, 4, 3);
	grid->addWidget(new QLabel(tr("Nombre d'objets de classe C :"),enabledE), 5, 0);
	grid->addWidget(itemClassCCountE, 5, 1);
	grid->addWidget(new QLabel(tr("Nombre d'objets de classe D :"),enabledE), 5, 2);
	grid->addWidget(itemClassDCountE, 5, 3);
	grid->addWidget(questGroupE, 6, 0, 1, 4, Qt::AlignTop);
	grid->addWidget(starCountGroupE, 7, 0, 1, 4, Qt::AlignTop);

	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(new QLabel(Data::names().at(BOKO)+tr(" :"),this), 0, 0);
	layout->addWidget(bokoE, 0, 1, 1, 3);
	layout->addLayout(pixLayout, 0, 4, 6, 1, Qt::AlignRight | Qt::AlignTop);
	layout->addWidget(enabledE, 1, 0, 1, 4);
	layout->setRowStretch(2, 1);
	layout->setColumnStretch(1, 1);
	layout->setColumnStretch(3, 1);
	layout->setContentsMargins(QMargins());

	connect(enabledE, SIGNAL(toggled(bool)), SLOT(setCWEnabled(bool)));
}

void CWEditor::fillPage()
{
	bokoE->setText(saveData->perso(BOKO));

	enabledE->setChecked(data->chocobo.enabled & 1);
	for (int i = 0; i < 7; ++i) {
		questE.at(i)->setChecked((data->chocobo.enabled >> (i + 1)) & 1);
	}

	for (int i = 0; i < 4; ++i) {
		if (data->chocobo.boko_attack == i) {
			starCountE.at(i)->setChecked(true);
			break;
		}
	}

	levelE->setValue(data->chocobo.level);
	currentHpE->setValue(data->chocobo.current_hp);
	maxHpE->setValue(data->chocobo.max_hp);

	int ID1 = data->chocobo.id_related & 0xF;
	int ID2 = (data->chocobo.id_related >> 4) & 0xF;
	int ID3 = (data->chocobo.id_related >> 8) & 0xF;

	if (ID1 > 9) {
		qWarning() << "CWEditor::fillPage invalid ID1";
		ID1 = 9;
	}
	if (ID2 > 9) {
		qWarning() << "CWEditor::fillPage invalid ID2";
		ID2 = 9;
	}
	if (ID3 > 9) {
		qWarning() << "CWEditor::fillPage invalid ID3";
		ID3 = 9;
	}

	idE->setValue(ID1 + 10*ID2 + 100*ID3);

	int W1 = data->chocobo.weapon & 0xF;
	int W2 = (data->chocobo.weapon >> 4) & 0xF;
	int W3 = (data->chocobo.weapon >> 8) & 0xF;
	int W4 = (data->chocobo.weapon >> 12) & 0xF;

	if (W1 > 9) {
		qWarning() << "CWEditor::fillPage invalid W1";
		W1 = 9;
	}
	if (W2 > 9) {
		qWarning() << "CWEditor::fillPage invalid W2";
		W2 = 9;
	}
	if (W3 > 9) {
		qWarning() << "CWEditor::fillPage invalid W3";
		W3 = 9;
	}
	if (W4 > 9) {
		qWarning() << "CWEditor::fillPage invalid W4";
		W4 = 9;
	}

	weaponE->setValue(W1 + 10*W2 + 100*W3 + 1000*W4);

	rankE->setValue(data->chocobo.rank + 1);
	saveCountE->setValue(data->chocobo.saveCount);

	itemClassACountE->setValue(data->chocobo.itemClassACount);
	itemClassBCountE->setValue(data->chocobo.itemClassBCount);
	itemClassCCountE->setValue(data->chocobo.itemClassCCount);
	itemClassDCountE->setValue(data->chocobo.itemClassDCount);
}

void CWEditor::savePage()
{
	saveData->setPerso(BOKO, bokoE->text());

	data->chocobo.enabled = enabledE->isChecked();
	for (int i = 0; i < 7; ++i) {
		data->chocobo.enabled |= questE.at(i)->isChecked() << (i + 1);
	}

	for (int i = 0; i < 4; ++i) {
		if (starCountE.at(i)->isChecked()) {
			data->chocobo.boko_attack = i;
			break;
		}
	}

	data->chocobo.level = levelE->value();
	data->chocobo.current_hp = currentHpE->value();
	data->chocobo.max_hp = maxHpE->value();

	int ID = idE->value(), ID3 = ID / 100, ID2 = ID / 10 - ID3 * 10, ID1 = ID - ID2 * 10 - ID3 * 100;
	data->chocobo.id_related = (data->chocobo.id_related & 0xF000) | (ID3 << 8) | (ID2 << 4) | ID1;

	int W = weaponE->value(), W4 = W / 1000, W3 = W / 100 - W4 * 10, W2 = W / 10 - W3 * 10 - W4 * 100, W1 = W - W2 * 10 - W3 * 100 - W4 * 1000;
	data->chocobo.weapon = (W4 << 12) | (W3 << 8) | (W2 << 4) | W1;

	data->chocobo.rank = rankE->value() - 1;
	data->chocobo.saveCount = saveCountE->value();

	data->chocobo.itemClassACount = itemClassACountE->value();
	data->chocobo.itemClassBCount = itemClassBCountE->value();
	data->chocobo.itemClassCCount = itemClassCCountE->value();
	data->chocobo.itemClassDCount = itemClassDCountE->value();
}

void CWEditor::setCWEnabled(bool enabled)
{
	for (QObject *child : enabledE->findChildren<QWidget *>()) {
		((QWidget *)child)->setEnabled(enabled);
	}
}
