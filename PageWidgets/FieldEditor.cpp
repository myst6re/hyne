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

#include "FieldEditor.h"

FieldEditor::FieldEditor(QWidget *parent) :
	PageWidget(parent)
{
}

void FieldEditor::buildWidget()
{
	QFont font;
	font.setPointSize(9);

	gameMomentList = new QTreeWidget(this);
	gameMomentList->setFont(font);
	gameMomentList->setUniformRowHeights(true);
	gameMomentList->setColumnCount(2);
	gameMomentList->setIndentation(0);
	gameMomentList->setHeaderLabels(QStringList() << tr("Valeur") << tr("Moment du jeu"));

	for(int i=0 ; i<403 ; ++i) {
		cInt couple = Data::momentLocation[i];

		int moment = couple.one, loc = couple.two;
		QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << QString::number(moment) << (loc >=0 ? Data::locations().value(loc) : tr("Disque %1").arg(-loc)));
		item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		item->setData(0, Qt::UserRole, moment);
		if(loc < 0) {
			item->setForeground(0, Qt::gray);
			item->setForeground(1, Qt::gray);
		}
		gameMomentList->addTopLevelItem(item);
	}

	gameMomentList->resizeColumnToContents(0);

	HelpWidget *info = new HelpWidget(32, tr("Cet éditeur ne change que le moment du jeu, c'est-à-dire une valeur "
											 "qui change en fonction de votre avancée dans l'aventure principale."
											 " En changeant cette valeur, la position de votre personnage, les sous-quêtes"
											 " et autres valeurs ne seront pas modifiées."), this);

	autoDiscE = new QCheckBox(tr("Auto."), this);

	discE = new QComboBox(this);
	for(int i=0 ; i<4 ; ++i) {
		discE->addItem(tr("Disque %1").arg(i+1), i);
	}

	QHBoxLayout *discL = new QHBoxLayout;
	discL->addStretch();
	discL->addWidget(autoDiscE);
	discL->addWidget(discE);
	discL->setContentsMargins(QMargins());

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(info);
	layout->addWidget(gameMomentList);
	layout->addLayout(discL);
	layout->setContentsMargins(QMargins());

	connect(gameMomentList, SIGNAL(itemClicked(QTreeWidgetItem*,int)), SLOT(adjustCheck(QTreeWidgetItem*)));
	connect(gameMomentList, SIGNAL(itemChanged(QTreeWidgetItem*,int)), SLOT(adjustCheck(QTreeWidgetItem*)));
	connect(autoDiscE, SIGNAL(toggled(bool)), discE, SLOT(setDisabled(bool)));

	autoDiscE->setChecked(true);
}

void FieldEditor::fillPage()
{
	gameMomentList->blockSignals(true);

	bool valueFound = false;
	int itemCount = gameMomentList->topLevelItemCount();
	QTreeWidgetItem *item;
	for(int i=0 ; i<itemCount ; ++i) {
		item = gameMomentList->topLevelItem(i);

		if(item->data(0, Qt::UserRole).toInt() == data->field.game_moment)
			valueFound = true;

		if(item->data(0, Qt::UserRole).toInt() <= data->field.game_moment)
			item->setCheckState(0, Qt::Checked);
		else if(!valueFound) {
			item = new QTreeWidgetItem(QStringList() << QString::number(data->field.game_moment) << tr("???"));
			item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
			item->setData(0, Qt::UserRole, data->field.game_moment);
			gameMomentList->insertTopLevelItem(i+1, item);
		}
		else
			item->setCheckState(0, Qt::Unchecked);
	}

	gameMomentList->resizeColumnToContents(0);

	gameMomentList->blockSignals(false);

	setCurrentIndex(discE, data->misc3.disc-1);
}

void FieldEditor::savePage()
{
	quint8 disc = discE->itemData(discE->currentIndex()).toInt();
	data->misc3.disc = disc + 1;
}

void FieldEditor::adjustCheck(QTreeWidgetItem *itemChanged)
{
	gameMomentList->blockSignals(true);

	Qt::CheckState checkState = Qt::Checked;
	int itemCount = gameMomentList->topLevelItemCount();
	QTreeWidgetItem *item;
	for(int i=0 ; i<itemCount ; ++i) {
		item = gameMomentList->topLevelItem(i);

		item->setCheckState(0, checkState);

		if(item == itemChanged) {
			checkState = Qt::Unchecked;
		}
	}

	data->field.game_moment = itemChanged->data(0, Qt::UserRole).toInt();

	if(autoDiscE->isChecked()) {
		setCurrentIndex(discE, int(data->field.game_moment>=395) + int(data->field.game_moment>=901) + int(data->field.game_moment>=3860));
	}

	gameMomentList->blockSignals(false);
}
