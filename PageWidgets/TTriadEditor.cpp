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

#include "TTriadEditor.h"

TTriadEditor::TTriadEditor(QWidget *parent)
	: PageWidget(parent)
{
}

void TTriadEditor::updateMode(bool mode)
{
	unknown1E_label->setVisible(mode);
	unknown1E->setVisible(mode);
	unknown2E_label->setVisible(mode);
	unknown2E->setVisible(mode);
}

void TTriadEditor::buildWidget()
{
	QVBoxLayout *layout = new QVBoxLayout(this);

	tabWidget = new QTabWidget(this);
	tabWidget->addTab(buildPage1(), tr("Cartes"));
	tabWidget->addTab(buildPage2(), tr("Règles"));

	layout->addWidget(tabWidget);
	layout->setContentsMargins(QMargins());
}

QWidget *TTriadEditor::buildPage1()
{
	QWidget *ret = new QWidget(this);

	QFont font;
	font.setPointSize(10);

	cardE_model = new QStandardItemModel(ret);
	cardE_view = new QTreeView(ret);
	cardE_view->setModel(cardE_model);
	cardE_view->setEditTriggers(QAbstractItemView::DoubleClicked);
	cardE_view->setFont(font);
	cardE_view->setItemDelegate(new SpinBoxDelegate(ret));
	cardE_view->setIndentation(0);
	cardE_view->setItemsExpandable(false);
	cardE_view->header()->setStretchLastSection(false);
	cardE_view->setUniformRowHeights(true);
	cardE_view->setAllColumnsShowFocus(true);

	cardE_model2 = new QStandardItemModel(ret);
	cardE_view2 = new QTreeView(ret);
	cardE_view2->setModel(cardE_model2);
	cardE_view2->setEditTriggers(QAbstractItemView::DoubleClicked);
	cardE_view2->setFont(font);
	cardE_view2->setItemDelegate(new SpinBoxDelegate(ret));
	cardE_view2->setIndentation(0);
	cardE_view2->setItemsExpandable(false);
	cardE_view2->header()->setStretchLastSection(false);
	cardE_view2->setUniformRowHeights(true);
	cardE_view2->setAllColumnsShowFocus(true);

	QStringList labels;
	QStandardItem *standardItem;
	QIcon cardIcon(":/images/icons/carte.png");

	labels << tr("Nom") << tr("Qté");
	cardE_model->setHorizontalHeaderLabels(labels);

	for(quint8 i=0 ; i<77 ; ++i)
	{
		if(i % 11 == 0) {
			QList<QStandardItem *> items;

			standardItem = new QStandardItem(tr("Niveau %1").arg(i/11 + 1));
			standardItem->setEditable(false);
			standardItem->setEnabled(false);
			standardItem->setCheckable(false);
			standardItem->setData(-1, Qt::UserRole+2);
			items.append(standardItem);

			standardItem = new QStandardItem();
			standardItem->setEditable(false);
			standardItem->setEnabled(false);
			standardItem->setCheckable(false);
			standardItem->setData(-1, Qt::UserRole+2);
			items.append(standardItem);

			cardE_model->appendRow(items);
		}

		QList<QStandardItem *> items;

		standardItem = new QStandardItem(Data::cards.at(i));
		standardItem->setEditable(false);
		standardItem->setCheckable(true);
		standardItem->setIcon(cardIcon);
		standardItem->setData(i, Qt::UserRole+2);
		items.append(standardItem);

		standardItem = new QStandardItem();
		standardItem->setData(i, Qt::UserRole+2);
		standardItem->setData(SPINBOX_127, Qt::UserRole);
		items.append(standardItem);

		cardE_model->appendRow(items);
	}

	cardE_view->header()->setResizeMode(0, QHeaderView::Stretch);
	cardE_view->header()->setResizeMode(1, QHeaderView::ResizeToContents);

	labels.replace(1, tr("Possesseur"));
	cardE_model2->setHorizontalHeaderLabels(labels);

	for(quint8 i=0 ; i<33 ; ++i)
	{
		if(i % 11 == 0) {
			QList<QStandardItem *> items;

			standardItem = new QStandardItem(tr("Niveau %1").arg(i/11 + 8));
			standardItem->setEditable(false);
			standardItem->setEnabled(false);
			standardItem->setCheckable(false);
			standardItem->setData(-1, Qt::UserRole+2);
			items.append(standardItem);

			standardItem = new QStandardItem();
			standardItem->setEditable(false);
			standardItem->setEnabled(false);
			standardItem->setCheckable(false);
			standardItem->setData(-1, Qt::UserRole+2);
			items.append(standardItem);

			cardE_model2->appendRow(items);
		}

		QList<QStandardItem *> items;

		standardItem = new QStandardItem(Data::cards.at(77+i));
		standardItem->setEditable(false);
		standardItem->setCheckable(true);
		standardItem->setIcon(cardIcon);
		standardItem->setData(i, Qt::UserRole+2);
		items.append(standardItem);

		standardItem = new QStandardItem();
		standardItem->setData(i, Qt::UserRole+2);
		standardItem->setData(COMBOBOX_LOCATIONS, Qt::UserRole);
		items.append(standardItem);

		cardE_model2->appendRow(items);
	}

	cardE_view2->header()->setResizeMode(0, QHeaderView::ResizeToContents);
	cardE_view2->header()->setResizeMode(1, QHeaderView::Stretch);

	font.setPixelSize(10);
	QString selectAll = tr("Sélectionner tout");
	QCheckBox *selectAll1 = new QCheckBox(selectAll, ret);
	QCheckBox *selectAll2 = new QCheckBox(selectAll, ret);
	selectAll1->setFont(font);
	selectAll2->setFont(font);
	QWidget *qtyAll = new QWidget;
	QPushButton *qtyAllOK = new QPushButton(tr("Tout"), qtyAll);
	QPushButton *squallAll = new QPushButton(tr("Squall"), ret);
	qtyAllOK->setFont(font);
	squallAll->setFont(font);
	qtyAllSpin = new QSpinBox(qtyAll);
	qtyAllSpin->setRange(0, 127);
	QHBoxLayout *qtyLayout = new QHBoxLayout(qtyAll);
	qtyLayout->addWidget(qtyAllSpin, 1);
	qtyLayout->addWidget(qtyAllOK);
	qtyLayout->setContentsMargins(QMargins());

	cardPreview = new QLabel(ret);
	setCardPreview(110);

	connect(selectAll1, SIGNAL(toggled(bool)), SLOT(selectAll1(bool)));
	connect(selectAll2, SIGNAL(toggled(bool)), SLOT(selectAll2(bool)));
	connect(qtyAllOK, SIGNAL(released()), SLOT(qtyAll()));
	connect(squallAll, SIGNAL(released()), SLOT(squallAll()));
	connect(cardE_view->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(changeCardPreview()));
	connect(cardE_view2->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(changeCardPreview2()));

	QLabel *info = new QLabel(tr("<b>Case cochée :</b> carte connue<br/><b>Possesseur :</b> joueur possédant la carte rare, mettre Squall pour l'avoir dans votre jeu."), ret);
	info->setTextFormat(Qt::RichText);
	QLabel *infoIcon = new QLabel(ret);
	infoIcon->setPixmap(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation).pixmap(16));
	QHBoxLayout *infoLayout = new QHBoxLayout;
	infoLayout->addWidget(infoIcon);
	infoLayout->addWidget(info);
	infoLayout->addStretch();

	QGridLayout *layout = new QGridLayout(ret);
	layout->addLayout(infoLayout, 0, 0, 1, 3, Qt::AlignTop | Qt::AlignLeft);
	layout->addWidget(cardPreview, 0, 3, 2, 1, Qt::AlignRight);
	layout->addWidget(cardE_view, 1, 0, 2, 2);
	layout->addWidget(selectAll1, 3, 0, Qt::AlignLeft);
	layout->addWidget(qtyAll, 3, 1, Qt::AlignRight);
	layout->addWidget(cardE_view2, 2, 2, 1, 2);
	layout->addWidget(selectAll2, 3, 2, Qt::AlignLeft);
	layout->addWidget(squallAll, 3, 3, Qt::AlignRight);

	return ret;
}

void TTriadEditor::changeCardPreview()
{
	int row = cardE_model->itemFromIndex(cardE_view->currentIndex())->data(Qt::UserRole+2).toInt();
	if(row != -1)
		setCardPreview(row);
}

void TTriadEditor::changeCardPreview2()
{
	int row = cardE_model2->itemFromIndex(cardE_view2->currentIndex())->data(Qt::UserRole+2).toInt();
	if(row != -1)
		setCardPreview(77 + row);
}

void TTriadEditor::setCardPreview(int i)
{
	if(i<0 || i>110)	return;
	cardPreview->setPixmap(QPixmap(":/images/cards.png").copy((i%14) * 62, (i/14) * 62, 62, 62));
}

QWidget *TTriadEditor::buildPage2()
{
	QStringList regions;
	regions << tr("Balamb") << tr("Galbadia")
			<< tr("Trabia") << tr("Centra") << tr("Dollet")
			<< tr("Horizon") << tr("Lunar Gate") << tr("Esthar");

	QWidget *ret = new QWidget(this);

	QFont font;
	font.setPointSize(9);

	ruleE_list = new QListWidget(ret);
	ruleE_list->setFont(font);
	ruleE_list->setUniformItemSizes(true);
	ruleE_list->addItems(QStringList() << regions << tr("Dernières règles (1)") << tr("Dernières règles (2)"));
	connect(ruleE_list, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), SLOT(fillRules(QListWidgetItem*,QListWidgetItem*)));

	QGroupBox *group1 = new QGroupBox(tr("Règles"), ret);
	QVBoxLayout *group1L = new QVBoxLayout(group1);
	QStringList rules_names;
	rules_names << tr("Open") << tr("Identique") << tr("Plus") << tr("Aléatoire") << tr("Mort subite") << tr("(Essai)") << tr("Même mur") << tr("Élémentaire");
	QCheckBox *rule;
	for(int i=0 ; i<8 ; ++i) {
		rule = new QCheckBox(rules_names.at(i), group1);
		group1L->addWidget(rule);
		rules.append(rule);
	}
	group1L->addStretch(1);

	group2 = new QGroupBox(tr("Règle du vainqueur"), ret);
	QVBoxLayout *group2L = new QVBoxLayout(group2);

	QStringList traderules_names;
	traderules_names << tr("Aucune") << tr("One") << tr("Change") << tr("Direct") << tr("Totale") << "???" << "???" << "???";
	QRadioButton *trade_rule;
	for(int i=0 ; i<8 ; ++i) {
		trade_rule = new QRadioButton(traderules_names.at(i), group2);
		group2L->addWidget(trade_rule);
		trade_rules.append(trade_rule);
	}
	group2L->addStretch(1);

	QGroupBox *group3 = new QGroupBox(tr("Divers"), ret);
	QGridLayout *group3L = new QGridLayout(group3);

	last_regionE = new QComboBox(group3);
	last_regionE->addItem(QString::number(0), 0);
	int i=1;
	foreach(const QString &region, regions)
		last_regionE->addItem(region, i++);

	last2_regionE = new QComboBox(group3);
	last2_regionE->addItem(QString::number(0), 0);
	i=1;
	foreach(const QString &region, regions)
		last2_regionE->addItem(region, i++);

	queenE = new QComboBox(group3);
	queenE->addItem(QString::number(0), 0);
	QStringList queenCities;
	queenCities << tr("Balamb City") << tr("Deling City") << tr("Shumi village") << tr("Winhill") << tr("Dollet") << tr("Horizon") << tr("Lunar Gate") << tr("Esthar City");
	i=1;
	foreach(const QString &queenCity, queenCities)
		queenE->addItem(queenCity, i++);

	traderatingE = new QSpinBox(group3);
	traderatingE->setRange(0, MAX_INT8);
	traderating_regionE = new QComboBox(group3);
	traderating_regionE->addItem(QString::number(0), 0);
	i=1;
	foreach(const QString &region, regions)
		traderating_regionE->addItem(region, i++);

	ttvictorycountE = new QSpinBox(group3);
	ttvictorycountE->setRange(0, MAX_INT16);
	ttdefeatcountE = new QSpinBox(group3);
	ttdefeatcountE->setRange(0, MAX_INT16);
	ttegalitycountE = new QSpinBox(group3);
	ttegalitycountE->setRange(0, MAX_INT16);

	ttdegenerationE = new QSpinBox(group3);
	ttdegenerationE->setRange(0, MAX_INT8);

	ttcardqueenquestE = new QComboBox(group3);
	ttcardqueenquestE->addItem(tr("MiniMog"), 0);
	ttcardqueenquestE->addItem(tr("Tauros"), 1);
	ttcardqueenquestE->addItem(tr("Chicobo"), 2);
	ttcardqueenquestE->addItem(tr("Alexander"), 3);
	ttcardqueenquestE->addItem(tr("Helltrain"), 4);
	ttcardqueenquestE->addItem(tr("Toutes"), 5);

	unknown1E_label = new QLabel(tr("Inconnu 1 :"),group3);
	unknown1E = new QSpinBox(group3);
	unknown1E->setRange(0, MAX_INT16);
	unknown2E_label = new QLabel(tr("Inconnu 2 :"),group3);
	unknown2E = new QDoubleSpinBox(group3);
	unknown2E->setDecimals(0);
	unknown2E->setRange(0, MAX_INT32);

	group3L->addWidget(new QLabel(tr("Dernières régions visitées :"),group3), 0, 0);
	group3L->addWidget(last_regionE, 0, 1);
	group3L->addWidget(last2_regionE, 0, 2);
	group3L->addWidget(new QLabel(tr("Emplacement reine des cartes :"),group3), 2, 0);
	group3L->addWidget(queenE, 2, 1);
	group3L->addWidget(new QLabel(tr("Nombre de joueurs règle du vainqueur :"),group3), 3, 0);
	group3L->addWidget(traderatingE, 3, 1);
	group3L->addWidget(new QLabel(tr("Région :"),group3), 3, 2);
	group3L->addWidget(traderating_regionE, 3, 3);
	group3L->addWidget(new QLabel(tr("Dégénération :"),group3), 4, 0);
	group3L->addWidget(ttdegenerationE, 4, 1);
	group3L->addWidget(new QLabel(tr("Cartes créées :"),group3), 4, 2);
	group3L->addWidget(ttcardqueenquestE, 4, 3);
	group3L->addWidget(new QLabel(tr("Nombre de victoires :"),group3), 5, 0);
	group3L->addWidget(ttvictorycountE, 5, 1);
	group3L->addWidget(new QLabel(tr("Nombre de défaites :"),group3), 5, 2);
	group3L->addWidget(ttdefeatcountE, 5, 3);
	group3L->addWidget(new QLabel(tr("Nombre d'égalités :"),group3), 6, 0);
	group3L->addWidget(ttegalitycountE, 6, 1);
	group3L->addWidget(unknown1E_label, 7, 0);
	group3L->addWidget(unknown1E, 7, 1);
	group3L->addWidget(unknown2E_label, 7, 2);
	group3L->addWidget(unknown2E, 7, 3);

	QGridLayout *layout = new QGridLayout(ret);
	layout->addWidget(ruleE_list, 0, 0);
	layout->addWidget(group1, 0, 1);
	layout->addWidget(group2, 0, 2);
	layout->addWidget(group3, 1, 0, 1, 4);
	layout->setRowStretch(2, 1);
	layout->setColumnStretch(3, 1);

	return ret;
}

void TTriadEditor::fillPage()
{
	QStandardItem *standardItem;
	
	for(quint8 i=0 ; i<cardE_model->rowCount() ; ++i)
	{
		int row = cardE_model->item(i)->data(Qt::UserRole+2).toInt();
		if(row != -1) {
			cardE_model->item(i)->setCheckState(data->ttcards.cards[row] >> 7 ? Qt::Checked : Qt::Unchecked);
			cardE_model->item(i, 1)->setText(QString::number(data->ttcards.cards[row] & 0x7F));
		}
	}
	
	for(quint8 i=0 ; i<cardE_model2->rowCount() ; ++i)
	{
		int row = cardE_model2->item(i)->data(Qt::UserRole+2).toInt();
		if(row != -1) {
			cardE_model2->item(i)->setCheckState(data->ttcards.cards_rare[row/8] & (1 << (row%8)) ? Qt::Checked : Qt::Unchecked);
			standardItem = cardE_model2->item(i, 1);
			standardItem->setText(Data::getCardsLocation(data->ttcards.card_locations[row]));
			standardItem->setData(data->ttcards.card_locations[row]);
		}
	}

	ruleE_list->setCurrentRow(0);
	setCurrentIndex(last_regionE, data->field.tt_lastregion[0]);
	setCurrentIndex(last2_regionE, data->field.tt_lastregion[1]);
	setCurrentIndex(queenE, data->field.tt_cardqueen_location);
	traderatingE->setValue(data->field.tt_traderating);
	setCurrentIndex(traderating_regionE, data->field.tt_traderating_region);
	ttvictorycountE->setValue(data->ttcards.tt_victory_count);
	ttdefeatcountE->setValue(data->ttcards.tt_defeat_count);
	ttegalitycountE->setValue(data->ttcards.tt_egality_count);
	ttdegenerationE->setValue(data->field.tt_degeneration);
	setCurrentIndex(ttcardqueenquestE, data->field.tt_cardqueen_quest);
	unknown1E->setValue(data->ttcards.u2);
	unknown2E->setValue(data->ttcards.u3);
}

void TTriadEditor::fillRules(QListWidgetItem *, QListWidgetItem *previous)
{
	int id = ruleE_list->currentRow(), i;
	saveRules(ruleE_list->row(previous));
	quint8 tt_rules;

	if(id<8)
	{
		tt_rules = data->field.tt_rules[id];
		for(i=0 ; i<8 ; ++i) {
			rules.at(i)->setChecked((tt_rules >> i) & 1);
		}

		tt_rules = data->field.tt_traderules[id];
		for(i=0 ; i<8 ; ++i) {
			trade_rules.at(i)->setChecked(tt_rules == i);
		}
		group2->show();
	}
	else if(id<10)
	{
		tt_rules = data->field.tt_lastrules[id-8];
		for(i=0 ; i<8 ; ++i) {
			rules.at(i)->setChecked((tt_rules >> i) & 1);
		}
		group2->hide();
	}
}

void TTriadEditor::savePage()
{
	quint8 value;
	quint8 exists;
	bool ok;
	for(quint8 i=0 ; i<cardE_model->rowCount() ; ++i)
	{
		int row = cardE_model->item(i)->data(Qt::UserRole+2).toInt();
		if(row != -1) {
			value = cardE_model->item(i, 1)->text().toUInt() & 0x7F;
			exists = (cardE_model->item(i, 0)->checkState() == Qt::Checked) << 7;
			data->ttcards.cards[row] = exists | value;
		}
	}
	for(quint8 i=0 ; i<cardE_model2->rowCount() ; ++i)
	{
		int row = cardE_model2->item(i)->data(Qt::UserRole+2).toInt();
		if(row != -1) {
			data->ttcards.cards_rare[row/8] = cardE_model2->item(i, 0)->checkState() == Qt::Checked ? ( data->ttcards.cards_rare[row/8] | (1 << (row%8)) ) : ( data->ttcards.cards_rare[row/8] & ~(1 << (row%8)) );
			value = cardE_model2->item(i, 1)->data().toUInt(&ok);
			if(ok)	data->ttcards.card_locations[row] = value;
		}
	}
	saveRules(ruleE_list->currentRow());
	data->field.tt_lastregion[0] = last_regionE->itemData(last_regionE->currentIndex()).toInt();
	data->field.tt_lastregion[1] = last2_regionE->itemData(last2_regionE->currentIndex()).toInt();
	data->field.tt_cardqueen_location = queenE->itemData(queenE->currentIndex()).toInt();
	data->field.tt_traderating = traderatingE->value();
	data->field.tt_traderating_region = traderating_regionE->itemData(traderating_regionE->currentIndex()).toInt();
	data->ttcards.tt_victory_count = ttvictorycountE->value();
	data->ttcards.tt_defeat_count = ttdefeatcountE->value();
	data->ttcards.tt_egality_count = ttegalitycountE->value();
	data->field.tt_degeneration = ttdegenerationE->value();
	data->field.tt_cardqueen_quest = ttcardqueenquestE->itemData(ttcardqueenquestE->currentIndex()).toInt();
	data->ttcards.u2 = unknown1E->value();
	data->ttcards.u3 = unknown2E->value();
}

void TTriadEditor::saveRules(int id)
{
	quint8 tt_rules;
	int i;

	if(id<0)	return;

	if(id<8)
	{
		tt_rules = 0;
		for(i=0 ; i<8 ; ++i) {
			tt_rules |= rules.at(i)->isChecked() << i;
		}

		data->field.tt_rules[id] = tt_rules;

		tt_rules = 0;
		for(i=0 ; i<8 ; ++i) {
			if(trade_rules.at(i)->isChecked()) {
				tt_rules = i;
				break;
			}
		}
		// Change 3 bit only
		data->field.tt_traderules[id] = (data->field.tt_traderules[id] & 0xF8) | tt_rules;
	}
	else if(id<10)
	{
		tt_rules = 0;
		for(i=0 ; i<8 ; ++i) {
			tt_rules |= rules.at(i)->isChecked() << i;
		}

		data->field.tt_lastrules[id-8] = tt_rules;
	}
}

void TTriadEditor::selectAll1(bool checked)
{
	for(quint8 i=0 ; i<cardE_model->rowCount() ; ++i)
	{
		if(cardE_model->item(i)->data(Qt::UserRole+2).toInt() != -1) {
			cardE_model->item(i)->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
		}
	}
}

void TTriadEditor::qtyAll()
{
	int qty = qtyAllSpin->value();
	for(quint8 i=0 ; i<cardE_model->rowCount() ; ++i)
	{
		if(cardE_model->item(i)->data(Qt::UserRole+2).toInt() != -1) {
			cardE_model->item(i, 1)->setText(QString::number(qty));
		}
	}
}

void TTriadEditor::selectAll2(bool checked)
{
	for(quint8 i=0 ; i<cardE_model2->rowCount() ; ++i)
	{
		if(cardE_model2->item(i)->data(Qt::UserRole+2).toInt() != -1) {
			cardE_model2->item(i)->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
		}
	}
}

void TTriadEditor::squallAll()
{
	for(quint8 i=0 ; i<cardE_model2->rowCount() ; ++i)
	{
		if(cardE_model2->item(i)->data(Qt::UserRole+2).toInt() != -1) {
			cardE_model2->item(i, 1)->setData(240);
			cardE_model2->item(i, 1)->setText(tr("Squall"));
		}
	}
}
