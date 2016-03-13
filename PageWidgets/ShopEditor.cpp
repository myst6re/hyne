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

#include "ShopEditor.h"

int ShopEditor::items[6][16] = {
	{ 36, 37, 39, 34, 55, 56, 57, 58, 41, 42, 43, 44, 45, 46, 191, 192 },// Pet Shop Timber
	{ 1, 3, 7, 11, 13, 12, 14, 15, 16, 33, 162, 101, 102, 36, 37, 39 },// All
	{ 3, 7, 13, 12, 16, 33, 162, 101, 102, 103, 104, 105, 106, 36, 37, 39 },// Shop Esthar
	{ 36, 37, 39, 34, 41, 42, 43, 44, 45, 46, 193, 194, 75, 78, 84, 87 },// Pet Shop Esthar
	{ 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 195, 196 },// Bookshop Esthar
	{ 1, 3, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 33, 35, 36 }// Shop Esthar!
};

QList<int> ShopEditor::rareItems[6];

ShopEditor::ShopEditor(QWidget *parent)
	: PageWidget(parent)
{
}

void ShopEditor::buildWidget()
{
	QFont font;
	font.setPointSize(9);

	shopList = new QListWidget(this);
	shopList->setFont(font);
	shopList->setUniformItemSizes(true);
	shopList->setFixedWidth(200);

	rareItems[0].clear();
	rareItems[0] << 9 << 10 << 11 << 12 << 13;// X-A Scrolls
	rareItems[1].clear();
	rareItems[1] << 14;// G-Hi-Potion
	rareItems[2].clear();
	rareItems[2] << 11 << 12;// Demolition Ammo, Fast Ammo
	rareItems[3].clear();
	rareItems[3] << 12 << 13 << 14 << 15;// Giant's Ring, Power Wrist, Force Armlet, Hypno Crown
	rareItems[4].clear();
	rareItems[4] << 0 << 10 << 11 << 14 << 15;// Weapons Mon 1st, Combat King 004/005, Occult Fan I/II
	rareItems[5].clear();
	rareItems[5] << 2 << 3 << 5 << 6 << 14;

	QStringList shops;
	shops << tr("Pet Shop de Timber") << tr("Boutique de Balamb") << tr("Boutique de Dollet")
		  << tr("Boutique de Timber") << tr("Boutique de Deling City") << tr("Boutique de Winhill")
		  << tr("Boutique d'Horizon") << tr("Boutique de Trabia ?") << tr("Boutique d'Esthar")
		  << tr("Boutique de Balamb ? (Laguna)") << tr("Boutique de Dollet ? (Laguna)") << tr("Boutique de Timber ? (Laguna)")
		  << tr("Boutique de Deling City ? (Laguna)") << tr("Boutique de Winhill (Laguna)") << tr("Boutique d'Horizon ? (Laguna)")
		  << tr("Boutique de Trabia ? (Laguna)") << tr("Garde de l'université") << tr("Pet Shop d'Esthar")
		  << tr("Librairie d'Esthar") << tr("Magasin d'Esthar!");

	shopList->addItems(shops);
	for(int i=0 ; i<20 ; ++i) {
		shopList->item(i)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	}

	normalItemColor = shopList->item(0)->foreground();

	shopEnabledE = new QCheckBox(tr("Magasin visité au moins une fois"), this);

	HelpWidget *info = new HelpWidget(16, tr("Les valeurs concernant les objets semblent ne pas être utilisées par le jeu."), this);

	itemList = new QListWidget(this);
	itemList->setFont(font);
	itemList->setUniformItemSizes(true);

	for(int i=0 ; i<16 ; ++i) {
		QListWidgetItem *item = new QListWidgetItem();
		item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		itemList->addItem(item);
	}

	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(shopList, 0, 0, 3, 1, Qt::AlignLeft);
	layout->addWidget(shopEnabledE, 0, 1);
	layout->addWidget(info, 1, 1);
	layout->addWidget(itemList, 2, 1);
	layout->setContentsMargins(QMargins());

	shopList->setCurrentRow(0);

	connect(shopList, SIGNAL(currentRowChanged(int)), SLOT(fillPage()));
	connect(shopEnabledE, SIGNAL(toggled(bool)), SLOT(setShopEnabled(bool)));
}

void ShopEditor::fillPage()
{
	savePage();

	int row = shopList->currentRow();

	if(row < 0) 	return;

	shops_data = &data->shops[row];

	shopEnabledE->setChecked(shops_data->visited & 1);

	QListWidgetItem *item;

	for(int i=0 ; i<20 ; ++i) {
		shopList->item(i)->setForeground(data->shops[i].visited ? normalItemColor : QColor(0x50,0x50,0x50));
	}

	int magType = getMagType();

	for(int i=0 ; i<16 ; ++i) {
		item = itemList->item(i);
		item->setText(Data::items().value(items[magType][i]));
		item->setIcon(QIcon(QString(":/images/icons/objet%1.png").arg(Data::itemType(items[magType][i]))));
		item->setCheckState(shops_data->visited && (shops_data->items[i] & 1) ? Qt::Checked : Qt::Unchecked);
		QFont itemFont = item->font();
		itemFont.setItalic(rareItems[magType].contains(i));
		item->setFont(itemFont);
	}
	itemList->setEnabled(shops_data->visited);
}

void ShopEditor::savePage()
{
	if(!loaded)	return;

	shops_data->visited = (shops_data->visited & 0xFE) | int(shopEnabledE->isChecked());

	if(shops_data->visited) {
		for(int i=0 ; i<16 ; ++i) {
			shops_data->items[i] = itemList->item(i)->checkState() == Qt::Checked;
		}
	} else {
		QByteArray its("\x64\x64\x32\x28\x1e\x14\x05\x03\x32\x32\x1e\x14\x0a\x05\x04\x01", 16);
		for(int i=0 ; i<16 ; ++i) {
			shops_data->items[i] = its[i];
		}
	}
}

void ShopEditor::setShopEnabled(bool enabled)
{
	int magType = getMagType();
	itemList->setEnabled(enabled);
	for(int i=0 ; i<16 ; ++i) {
		itemList->item(i)->setCheckState(enabled && !rareItems[magType].contains(i) ? Qt::Checked : Qt::Unchecked);
	}
	shopList->currentItem()->setForeground(enabled ? normalItemColor : QColor(0x40,0x40,0x40));
}

int ShopEditor::getMagType()
{
	switch(shopList->currentRow()) {
	case 0:		return 0;
	case 8:		return 2;
	case 17:	return 3;
	case 18:	return 4;
	case 19:	return 5;
	default:	return 1;
	}
}
