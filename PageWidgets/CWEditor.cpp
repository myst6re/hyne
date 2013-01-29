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
#include "FF8Text.h"

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

	levelE = new QSpinBox(this);
	levelE->setRange(0, MAX_INT8);
	currentHpE = new QSpinBox(this);
	currentHpE->setRange(0, MAX_INT8);
	maxHpE = new QSpinBox(this);
	maxHpE->setRange(0, MAX_INT8);
	idE = new QSpinBox(this);
	idE->setRange(000, 999);
	weaponE = new QSpinBox(this);
	weaponE->setRange(0000, 9999);

//	QGroupBox *questGroupE = new QGroupBox(tr("Avancement quête"), this);
//	QGridLayout *questL = new QGridLayout(questGroupE);

//	QStringList texts;
//	texts << tr("Activé") << tr("MiniMog trouvé") << tr("Koko rencontrée") << tr("Dépêche-toi !") << tr("Koko enlevée") << tr("Roi démon vaincu") << tr("Boko et MiniMog : Home/World") << tr("MiniMog disponible");
//	for(int i=0 ; i<8 ; ++i)
//	{
//		QCheckBox *questCheckBox = new QCheckBox(texts.at(i), questGroupE);
//		questE.append(questCheckBox);
//		questL->addWidget(questCheckBox, i/4, i%4);
//	}

	QGridLayout *grid = new QGridLayout(this);
	grid->addWidget(new QLabel(Data::names().at(BOKO)+tr(" :"),this), 0, 0);
	grid->addWidget(bokoE, 0, 1);
	grid->addWidget(new QLabel(tr("ID :"),this), 0, 2);
	grid->addWidget(idE, 0, 3);
	grid->addLayout(pixLayout, 0, 4, 5, 1, Qt::AlignRight | Qt::AlignTop);
	grid->addWidget(new QLabel(tr("Niveau :"),this), 1, 0);
	grid->addWidget(levelE, 1, 1);
	grid->addWidget(new QLabel(tr("Arme :"),this), 1, 2);
	grid->addWidget(weaponE, 1, 3);
	grid->addWidget(new QLabel(tr("HP actuels :"),this), 2, 0);
	grid->addWidget(currentHpE, 2, 1);
	grid->addWidget(new QLabel(tr("HP max :"),this), 2, 2);
	grid->addWidget(maxHpE, 2, 3);
//	grid->addWidget(questGroupE, 3, 0, 1, 4);
	grid->setRowStretch(3, 1);
	grid->setColumnStretch(1, 1);
	grid->setColumnStretch(3, 1);
	grid->setContentsMargins(QMargins());
}

void CWEditor::fillPage()
{
	bokoE->setText(FF8Text::toString(descData->boko, saveData->isJp()));

//	for(int i=0 ; i<8 ; ++i)
//		questE.at(i)->setChecked((data->chocobo.enabled >> i) & 1);

	levelE->setValue(data->chocobo.level);
	currentHpE->setValue(data->chocobo.current_hp);
	maxHpE->setValue(data->chocobo.max_hp);

	int ID1 = qMin(data->chocobo.id_related & 0xF, 9);
	int ID2 = qMin((data->chocobo.id_related >> 4) & 0xF, 9);
	int ID3 = qMin((data->chocobo.id_related >> 8) & 0xF, 9);
	idE->setValue(ID1 + 10*ID2 + 100*ID3);

	int W1 = qMin(data->chocobo.weapon & 0xF, 9);
	int W2 = qMin((data->chocobo.weapon >> 4) & 0xF, 9);
	int W3 = qMin((data->chocobo.weapon >> 8) & 0xF, 9);
	int W4 = qMin((data->chocobo.weapon >> 12) & 0xF, 9);
	weaponE->setValue(W1 + 10*W2 + 100*W3 + 1000*W4);
}

void CWEditor::savePage()
{
	memcpy(&descData->boko, FF8Text::toByteArray(bokoE->text(), saveData->isJp()).leftJustified(11, '\x00', true).append('\x00').constData(), 12);

//	data->chocobo.enabled = 0;
//	for(int i=0 ; i<8 ; ++i)
//		data->chocobo.enabled |= questE.at(i)->isChecked() << i;

	data->chocobo.level = levelE->value();
	data->chocobo.current_hp = currentHpE->value();
	data->chocobo.max_hp = maxHpE->value();

	int ID = idE->value(), ID3 = ID / 100, ID2 = ID / 10 - ID3 * 10, ID1 = ID - ID2 * 10 - ID3 * 100;
	data->chocobo.id_related = (data->chocobo.id_related & 0xF000) | (ID3 << 8) | (ID2 << 4) | ID1;

	int W = weaponE->value(), W4 = W / 1000, W3 = W / 100 - W4 * 10, W2 = W / 10 - W3 * 10 - W4 * 100, W1 = W - W2 * 10 - W3 * 100 - W4 * 1000;
	data->chocobo.weapon = (W4 << 12) | (W3 << 8) | (W2 << 4) | W1;
}
