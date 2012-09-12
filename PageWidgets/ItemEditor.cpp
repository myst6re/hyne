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

#include "ItemEditor.h"

ItemEditor::ItemEditor(QWidget *parent)
	: PageWidget(parent)
{
}

void ItemEditor::buildWidget()
{
	QTabWidget *tabWidget = new QTabWidget(this);
	tabWidget->addTab(buildPage1(), tr("Objets"));
	tabWidget->addTab(buildPage2(), tr("Ordre en combat"));
	tabWidget->addTab(buildPage3(), tr("Magazines"));

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(tabWidget);
	layout->setContentsMargins(QMargins());
}

QWidget *ItemEditor::buildPage1()
{
	QWidget *ret = new QWidget(this);

	QFont font;
	font.setPointSize(10);
	itemE_model = new QStandardItemModel(ret);
	itemE_view = new QTreeView(ret);
	itemE_view->setModel(itemE_model);
	itemE_view->setEditTriggers(QAbstractItemView::DoubleClicked);
	itemE_view->setFont(font);
	itemE_view->setItemDelegate(new SpinBoxDelegate(ret));
	itemE_view->setIndentation(0);
	itemE_view->setItemsExpandable(false);
	itemE_view->setUniformRowHeights(true);
	itemE_view->header()->setStretchLastSection(false);
	itemE_view->setAllColumnsShowFocus(true);

	QWidget *buttonsW = new QWidget(ret);

	QPushButton *allItems = new QPushButton(tr("Tous les objets"), buttonsW);
	QPushButton *type_sort = new QPushButton(tr("Type"), buttonsW);
	QPushButton *alpha_sort = new QPushButton(tr("Ordre alphabétique"), buttonsW);

	QHBoxLayout *buttons = new QHBoxLayout(buttonsW);
	buttons->addWidget(allItems);
	buttons->addStretch();
	buttons->addWidget(new QLabel(tr("Trier par :"), buttonsW));
	buttons->addWidget(type_sort);
	buttons->addWidget(alpha_sort);
	buttons->setContentsMargins(QMargins());
	
	QVBoxLayout *layout = new QVBoxLayout(ret);
	layout->addWidget(itemE_view);
	layout->addWidget(buttonsW);

	connect(allItems, SIGNAL(released()), SLOT(allItems()));
	connect(type_sort, SIGNAL(released()), SLOT(sortByType()));
	connect(alpha_sort, SIGNAL(released()), SLOT(sortByAlpha()));

	return ret;
}

QWidget *ItemEditor::buildPage2()
{
	QWidget *ret = new QWidget(this);

	QFont font;
	font.setPointSize(10);

	battle_itemE_list = new QListWidget(ret);
	battle_itemE_list->setFont(font);
	battle_itemE_list->setDragDropMode(QAbstractItemView::InternalMove);
	battle_itemE_list->setUniformItemSizes(true);

	QLabel *info = new QLabel(tr("Déplacez les éléments à la souris pour modifier l'ordre des objets en combat."), ret);
	info->setTextFormat(Qt::PlainText);
	QLabel *infoIcon = new QLabel(ret);
	infoIcon->setPixmap(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation).pixmap(16));
	QHBoxLayout *infoLayout = new QHBoxLayout;
	infoLayout->addWidget(infoIcon);
	infoLayout->addWidget(info);
	infoLayout->addStretch();

	QVBoxLayout *layout = new QVBoxLayout(ret);
	layout->addLayout(infoLayout);
	layout->addWidget(battle_itemE_list);

	return ret;
}

QWidget *ItemEditor::buildPage3()
{
	QWidget *ret = new QWidget(this);

	QFont font;
	font.setPointSize(10);
	QTreeWidgetItem *item;
	weaponsE_list = new QTreeWidget(ret);
	weaponsE_list->setFont(font);
	weaponsE_list->setHeaderLabel(tr("Fanzine des Fanas de Flingues"));
	weaponsE_list->setIndentation(0);
	weaponsE_list->setUniformRowHeights(true);

	for(int i=0 ; i<28 ; ++i) {
		item = new QTreeWidgetItem(QStringList(Data::weapons.at(i)));
		item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		item->setIcon(0, QIcon(QString(":/images/icons/weapon%1.png").arg(i,2,10,QChar('0'))));
		weaponsE_list->addTopLevelItem(item);
	}
	for(int i=28 ; i<32 ; ++i) {
		item = new QTreeWidgetItem(QStringList(tr("Inutilisé")));
		item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		weaponsE_list->addTopLevelItem(item);
	}

	QCheckBox *weaponsCheckAll = new QCheckBox(tr("Sélectionner tout"), ret);
	connect(weaponsCheckAll, SIGNAL(toggled(bool)), SLOT(selectAllWeapons(bool)));

	timbermaniacsE_list = new QTreeWidget(ret);
	timbermaniacsE_list->setFont(font);
	timbermaniacsE_list->setHeaderLabel(tr("Timber Maniacs"));
	timbermaniacsE_list->setIndentation(0);
	timbermaniacsE_list->setUniformRowHeights(true);

	QStringList timbermaniacsStrings;
	timbermaniacsStrings << tr("Hôtel de Balamb") << tr("Gare de Balamb") << tr("Pub de Dollet") << tr("Hôtel de Dollet")
						 << tr("Bureau de rédaction de Timber") << tr("Hôtel de Timber") << tr("Hôtel de Deling City") << tr("Horizon : Maison du bricoleur")
						 << tr("Hôtel d'Horizon") << tr("Université de Trabia - Cimetière") << tr("Ruines de Centra (automatique)") << tr("Village Shumi : Maison du bricoleur")
						 << tr("Orphelinat d'Edea") << tr("Bateau des seeds blancs") << tr("Inutilisé") << tr("Inutilisé");

	foreach(QString timbermaniacsString, timbermaniacsStrings) {
		item = new QTreeWidgetItem(QStringList(timbermaniacsString));
		item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		timbermaniacsE_list->addTopLevelItem(item);
	}

	QCheckBox *timbermaniacsCheckAll = new QCheckBox(tr("Sélectionner tout"), ret);
	connect(timbermaniacsCheckAll, SIGNAL(toggled(bool)), SLOT(selectAllTimberManiacs(bool)));

	font.setPixelSize(10);
	weaponsCheckAll->setFont(font);
	timbermaniacsCheckAll->setFont(font);

	QGridLayout *layout = new QGridLayout(ret);
	layout->addWidget(weaponsE_list, 0, 0);
	layout->addWidget(weaponsCheckAll, 1, 0, Qt::AlignLeft);
	layout->addWidget(timbermaniacsE_list, 0, 1);
	layout->addWidget(timbermaniacsCheckAll, 1, 1, Qt::AlignLeft);

	return ret;
}

void ItemEditor::fillPage()
{
	quint8 itemID;
	QStandardItem *standardItem;
	QListWidgetItem *item;
	QList<quint8> items;
	QMultiMap<int, int> battle_order;
	
	itemE_model->clear();
	itemE_model->setHorizontalHeaderLabels(QStringList() << tr("Nom") << tr("Qté"));
	int i;
	for(i=0 ; i<198 ; ++i)
	{
		itemID = data->items.items[i] & 0xFF;

		if(itemID != 0)		items.append(itemID);
		
		QList<QStandardItem *> items;
		standardItem = new QStandardItem(Data::items.value(itemID, QString::number(itemID)));
		standardItem->setData(COMBOBOX_ITEMS, Qt::UserRole);
		standardItem->setData(itemID);
		standardItem->setIcon(itemID == 0 ? QIcon() : QIcon(QString(":/images/icons/objet%1.png").arg(Data::itemType(itemID))));
		items.append(standardItem);
		
		standardItem = new QStandardItem(QString::number(data->items.items[i] >> 8));
		standardItem->setData(SPINBOX_256, Qt::UserRole);
		items.append(standardItem);
		itemE_model->appendRow(items);
	}
	
	battle_itemE_list->clear();
	itemE_view->header()->setResizeMode(0, QHeaderView::Stretch);
	itemE_view->header()->setResizeMode(1, QHeaderView::ResizeToContents);

	for(itemID=0 ; itemID<32 ; ++itemID)
	{
		// (pos, id)
		battle_order.insert(data->items.battle_order[itemID], itemID+1);
	}

	foreach(int itemID2, battle_order)
	{
		item = new QListWidgetItem(QIcon(QString(":/images/icons/objet%1.png").arg(Data::itemType(itemID2))),
								   itemID2 < 33 ? Data::items.at(itemID2) : QString::number(itemID2));
		if(!items.contains(itemID2))
			item->setForeground(Qt::darkGray);
		item->setData(Qt::UserRole, itemID2);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled);

		battle_itemE_list->addItem(item);
	}

	for(i=0 ; i<32 ; ++i) {
		weaponsE_list->topLevelItem(i)
				->setCheckState(0, (data->misc1.unlocked_weapons>>i)&1 ? Qt::Checked : Qt::Unchecked);
	}

	for(i=0 ; i<16 ; ++i) {
		timbermaniacsE_list->topLevelItem(i)
				->setCheckState(0, (data->field.timber_maniacs>>i)&1 ? Qt::Checked : Qt::Unchecked);
	}
}

void ItemEditor::savePage()
{
	int itemID, i;

	for(i=0 ; i<198 ; ++i)
	{
		data->items.items[i] = (quint8)itemE_model->item(i, 0)->data().toInt();
		data->items.items[i] |= (quint8)itemE_model->item(i, 1)->text().toUInt() << 8;
	}

	for(int pos=0 ; pos<32 ; ++pos)
	{
		itemID = battle_itemE_list->item(pos)->data(Qt::UserRole).toInt()-1;
		data->items.battle_order[itemID] = pos;
	}

	quint32 unlocked_weapons=0;
	for(i=0 ; i<32 ; ++i) {
		unlocked_weapons |= (weaponsE_list->topLevelItem(i)->checkState(0)==Qt::Checked)<<i;
	}
	data->misc1.unlocked_weapons = unlocked_weapons;

	quint16 timber_maniacs=0;
	for(i=0 ; i<16 ; ++i) {
		timber_maniacs |= (timbermaniacsE_list->topLevelItem(i)->checkState(0)==Qt::Checked)<<i;
	}
	data->field.timber_maniacs = timber_maniacs;
}

void ItemEditor::allItems()
{
	QStandardItem *item;
	for(int itemID=1 ; itemID<199 ; ++itemID)
	{
		item = itemE_model->item(itemID-1, 0);
		item->setData(itemID);
		item->setText(Data::items.at(itemID));
		item->setIcon(QIcon(QString(":/images/icons/objet%1.png").arg(Data::itemType(itemID))));
		itemE_model->item(itemID-1, 1)->setText(QString::number(100));
	}
}

void ItemEditor::sortByType()
{
	QMultiMap<int, quint8> items;
	int itemID, i;

	for(i=0 ; i<198 ; ++i)
	{
		itemID = itemE_model->item(i, 0)->data().toInt();
		if(itemID == 0)		itemID = 256;
		items.insert(itemID, itemE_model->item(i, 1)->text().toUInt());
	}

	QMultiMap<int, quint8>::const_iterator it = items.constBegin();

	i = 0;
	while(it != items.constEnd())
	{
		itemID = it.key();
		if(itemID == 256)		itemID = 0;
		data->items.items[i++] = itemID | (it.value() << 8);
		++it;
	}

	fillPage();
}

void ItemEditor::sortByAlpha()
{
	QMultiMap<QString, quint16> items;
	QStandardItem *item;
	int i;
	QString text;

	for(i=0 ; i<198 ; ++i)
	{
		item = itemE_model->item(i, 0);
		text = item->text();
		if(text.isEmpty())	text = "zzzzzzzzzzzzzzzzzz";
		items.insert(text, item->data().toInt() | (itemE_model->item(i, 1)->text().toUInt() << 8));
	}

	i = 0;
	foreach(quint16 value, items)
	{
		data->items.items[i++] = value;
	}

	fillPage();
}

void ItemEditor::selectAllWeapons(bool selected)
{
	PageWidget::selectAll(weaponsE_list, selected);
}

void ItemEditor::selectAllTimberManiacs(bool selected)
{
	PageWidget::selectAll(timbermaniacsE_list, selected);
}
