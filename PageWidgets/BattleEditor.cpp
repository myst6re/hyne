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

#include "BattleEditor.h"

BattleEditor::BattleEditor(QWidget *parent)
	: PageWidget(parent)
{
}

void BattleEditor::updateMode(bool mode)
{
	unknownGroupE->setVisible(mode);
}

void BattleEditor::buildWidget()
{
	QGroupBox *statsE = new QGroupBox(tr("Statistiques"), this);

	battlewinE = new QDoubleSpinBox(statsE);
	battlewinE->setDecimals(0);
	battlewinE->setRange(0, MAX_INT32);
	battleescE = new QSpinBox(statsE);
	battleescE->setRange(0, MAX_INT16);

	monsterkillsE = new QDoubleSpinBox(statsE);
	monsterkillsE->setDecimals(0);
	monsterkillsE->setRange(0, MAX_INT32);
	tombyE = new QDoubleSpinBox(statsE);
	tombyE->setDecimals(0);
	tombyE->setRange(0, MAX_INT32);

	tombySrE = new QCheckBox(tr("Tomberry Sr vaincu"), statsE);

	firstr1E = new QCheckBox(tr("Premier Elmidea"), statsE);
	firsteleE = new QCheckBox(tr("Premier Succube"), statsE);
	firstmtlE = new QCheckBox(tr("Premier T-Rex"), statsE);
	firstirvinelbE = new QCheckBox(tr("Premier combat avec Irvine"), statsE);

	QGridLayout *statsL = new QGridLayout(statsE);
	statsL->addWidget(new QLabel(tr("Combats gagnés :"),statsE), 0, 0, 1, 3);
	statsL->addWidget(battlewinE, 0, 3, 1, 3);
	statsL->addWidget(new QLabel(tr("Combats fuis :"),statsE), 0, 6, 1, 3);
	statsL->addWidget(battleescE, 0, 9, 1, 3);
	statsL->addWidget(new QLabel(tr("Monstres tués :"),statsE), 1, 0, 1, 3);
	statsL->addWidget(monsterkillsE, 1, 3, 1, 3);
	statsL->addWidget(new QLabel(tr("Tomberry tués :"),statsE), 1, 6, 1, 3);
	statsL->addWidget(tombyE, 1, 9, 1, 3);
	statsL->addWidget(tombySrE, 2, 0, 1, 4);
	statsL->addWidget(firstr1E, 2, 4, 1, 4);
	statsL->addWidget(firsteleE, 2, 8, 1, 4);
	statsL->addWidget(firstmtlE, 3, 0, 1, 4);
	statsL->addWidget(firstirvinelbE, 3, 4, 1, 4);

	unknownGroupE = new QGroupBox(tr("Inconnu"), this);

	unknown1E = new QDoubleSpinBox(unknownGroupE);
	unknown1E->setDecimals(0);
	unknown1E->setRange(0, MAX_INT32);

	unknown2E = new QDoubleSpinBox(unknownGroupE);
	unknown2E->setDecimals(0);
	unknown2E->setRange(0, MAX_INT32);

	QGridLayout *unknownL = new QGridLayout(unknownGroupE);
	unknownL->addWidget(new QLabel(tr("Inconnu 1 :"),statsE), 0, 0);
	unknownL->addWidget(unknown1E, 0, 1);
	unknownL->addWidget(new QLabel(tr("Inconnu 2 :"),statsE), 0, 2);
	unknownL->addWidget(unknown2E, 0, 3);

	QFont font;
	font.setPointSize(10);

	firstdrawE_list = new QTreeWidget(this);
	firstdrawE_list->setFont(font);
	firstdrawE_list->setHeaderLabel(tr("Magies vues au moins une fois"));
	firstdrawE_list->setIndentation(0);
	firstdrawE_list->setUniformRowHeights(true);

	QTreeWidgetItem *item;
	int i, size = Data::magic.size();
	for(i=1 ; i<size ; ++i) {
		item = new QTreeWidgetItem(QStringList(Data::magic.at(i)));
		item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		firstdrawE_list->addTopLevelItem(item);
	}

	for(i=size ; i<65 ; ++i) {
		item = new QTreeWidgetItem(QStringList(tr("Inutilisé")));
		item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		firstdrawE_list->addTopLevelItem(item);
	}

	QCheckBox *firstdrawCheckAll = new QCheckBox(tr("Sélectionner tout"), this);
	connect(firstdrawCheckAll, SIGNAL(toggled(bool)), SLOT(selectAllDraw(bool)));

	firstscanE_list = new QTreeWidget(this);
	firstscanE_list->setFont(font);
	firstscanE_list->setHeaderLabel(tr("Scannés au moins une fois"));
	firstscanE_list->setIndentation(0);
	firstscanE_list->setUniformRowHeights(true);

	for(i=SQUALL ; i<=WARD ; ++i) {
		item = new QTreeWidgetItem(QStringList(Data::names.at(i)));
		item->setIcon(0, QIcon(QString(":/images/icons/perso%1.png").arg(i)));
		item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		firstscanE_list->addTopLevelItem(item);
	}

	for(i=WARD+1 ; i<16 ; ++i) {
		item = new QTreeWidgetItem(QStringList(tr("Inutilisé")));
		item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		firstscanE_list->addTopLevelItem(item);
	}

	foreach(QString ennemy, Data::ennemies) {
		item = new QTreeWidgetItem(QStringList(ennemy));
		item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		firstscanE_list->addTopLevelItem(item);
	}

	for(i=16+Data::ennemies.size() ; i<160 ; ++i) {
		item = new QTreeWidgetItem(QStringList(tr("Inutilisé")));
		item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		firstscanE_list->addTopLevelItem(item);
	}

	QCheckBox *firstscanCheckAll = new QCheckBox(tr("Sélectionner tout"), this);
	connect(firstscanCheckAll, SIGNAL(toggled(bool)), SLOT(selectAllScan(bool)));

	font.setPixelSize(10);
	firstdrawCheckAll->setFont(font);
	firstscanCheckAll->setFont(font);

	QGridLayout *grid = new QGridLayout(this);
	grid->addWidget(statsE, 0, 0, 1, 2);
	grid->addWidget(unknownGroupE, 1, 0, 1, 2);
	grid->addWidget(firstdrawE_list, 2, 0);
	grid->addWidget(firstdrawCheckAll, 3, 0, Qt::AlignLeft);
	grid->addWidget(firstscanE_list, 2, 1);
	grid->addWidget(firstscanCheckAll, 3, 1, Qt::AlignLeft);
	QMargins margins = grid->contentsMargins();
	margins.setTop(0);
	margins.setBottom(0);
	grid->setContentsMargins(margins);
}

void BattleEditor::fillPage()
{
	battlewinE->setValue(data->misc2.victory_count);
	battleescE->setValue(data->misc2.battle_escaped);
	monsterkillsE->setValue(data->misc3.monster_kills);
	tombyE->setValue(data->misc2.tomberry_vaincus);
	tombySrE->setChecked(data->misc2.tomberry_sr_vaincu & 1);
	firstr1E->setChecked(data->misc2.elmidea_battle_r1 & 1);
	firsteleE->setChecked(data->misc2.succube_battle_elemental & 1);
	firstmtlE->setChecked(data->misc2.trex_battle_mental & 1);
	firstirvinelbE->setChecked(data->misc2.battle_irvine & 1);
	unknown1E->setValue(data->misc2.u3);
	unknown2E->setValue(data->misc2.u4);

	for(int i=0 ; i<64 ; ++i) {
		firstdrawE_list->topLevelItem(i)->setCheckState(0, (data->misc2.magic_drawn_once[i/8] >> (i%8)) & 1 ? Qt::Checked : Qt::Unchecked);
	}

	for(int i=0 ; i<160 ; ++i) {
		firstscanE_list->topLevelItem(i)->setCheckState(0, (data->misc2.ennemy_scanned_once[i/8] >> (i%8)) & 1 ? Qt::Checked : Qt::Unchecked);
	}
}

void BattleEditor::savePage()
{
	data->misc2.victory_count				= battlewinE->value();
	data->misc3.victory_count				= battlewinE->value();
	data->misc2.battle_escaped				= battleescE->value();
	data->misc3.battle_escaped				= battleescE->value();
	data->misc3.monster_kills				= monsterkillsE->value();
	data->misc2.tomberry_vaincus			= tombyE->value();
	data->misc2.tomberry_sr_vaincu			= (data->misc2.tomberry_sr_vaincu & 0xFFFE) | tombySrE->isChecked();
	data->misc2.elmidea_battle_r1			= (data->misc2.elmidea_battle_r1 & 0xFFFE) | firstr1E->isChecked();
	data->misc2.succube_battle_elemental	= (data->misc2.succube_battle_elemental & 0xFFFE) | firsteleE->isChecked();
	data->misc2.trex_battle_mental			= (data->misc2.trex_battle_mental & 0xFFFE) | firstmtlE->isChecked();
	data->misc2.battle_irvine				= (data->misc2.battle_irvine & 0xFFFE) | firstirvinelbE->isChecked();
	data->misc2.u3							= unknown1E->value();
	data->misc2.u4							= unknown2E->value();

	int i, j;
	quint8 cur;

	for(i=0 ; i<8 ; ++i) {
		cur = 0;
		for(j=0 ; j<8 ; ++j) {
			cur |= (firstdrawE_list->topLevelItem(i*8+j)->checkState(0)==Qt::Checked)<<j;
		}
		data->misc2.magic_drawn_once[i] = cur;
	}

	for(i=0 ; i<20 ; ++i) {
		cur = 0;
		for(j=0 ; j<8 ; ++j) {
			cur |= (firstscanE_list->topLevelItem(i*8+j)->checkState(0)==Qt::Checked)<<j;
		}
		data->misc2.ennemy_scanned_once[i] = cur;
	}
}

void BattleEditor::selectAllDraw(bool selected)
{
	PageWidget::selectAll(firstdrawE_list, selected);
}

void BattleEditor::selectAllScan(bool selected)
{
	PageWidget::selectAll(firstscanE_list, selected);
}
