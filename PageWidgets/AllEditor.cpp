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

#include "AllEditor.h"

AllEditor::AllEditor(QWidget *parent)
	: PageWidget(parent)
{
}

void AllEditor::buildWidget()
{
	tabBar = new QTabBar(this);
	tabBar->setDrawBase(false);
	tabBar->addTab(tr("Tout"));
//	tabBar->addTab(tr("En-tête"));
//	tabBar->addTab(tr("Divers 1"));
	tabBar->addTab(tr("Caméra"));
	tabBar->addTab(tr("Boutiques"));
	tabBar->addTab(tr("Divers"));
	tabBar->addTab(tr("Field"));
	tabBar->addTab(tr("Mappemonde"));
	tabBar->addTab(tr("Chocobo World"));
//	tabBar->addTab(tr("G-Forces"));
//	tabBar->addTab(tr("Personnages"));
//	tabBar->addTab(tr("Limit Break"));
//	tabBar->addTab(tr("Objets"));
//	tabBar->addTab(tr("Config"));
	hexEdit = new QHexEdit(this);

	QVBoxLayout *dLayout = new QVBoxLayout(this);
	dLayout->addWidget(tabBar);
	dLayout->addWidget(hexEdit);
	dLayout->setSpacing(0);
	dLayout->setContentsMargins(QMargins());

	connect(tabBar, SIGNAL(currentChanged(int)), SLOT(fillPage()));
	connect(hexEdit, SIGNAL(dataChanged()), SLOT(savePage()));
}

void AllEditor::fillPage()
{
	char *cur;

	switch(tabBar->currentIndex()) {
//	case 1:// Header
//		hexEdit->setData(QByteArray((char *)descData, sizeof(*descData)));
//		break;
//	case 1:// Misc 1
//		cur = (char *)data + 2724;
//		hexEdit->setData(QByteArray(cur, 32));
//		break;
	case 1:// Misc 2/Positions
		cur = (char *)data + /*3200*/3308;
		hexEdit->setData(QByteArray(cur, /*144*/36));
		break;
	case 2:// Shops
		cur = (char *)data + 2304;
		hexEdit->setData(QByteArray(cur, 400));
		break;
	case 3:// Misc
		cur = (char *)data + 3344;
		hexEdit->setData(QByteArray(cur, 256));
		break;
	case 4:// Field Vars
		cur = (char *)data + 3600;
		hexEdit->setData(QByteArray(cur, 1024));
		break;
	case 5:// WorldMap Vars
		cur = (char *)data + 4624;
		hexEdit->setData(QByteArray(cur, 128));
		break;
	case 6:// Chocobo World
		cur = (char *)data + 4880;
		hexEdit->setData(QByteArray(cur, 64));
		break;
	/*case 6:// GF
		hexEdit->setData(QByteArray((char *)data, 1088));
		break;
	case 7:// Characters
		cur = (char *)data + 1088;
		hexEdit->setData(QByteArray(cur, 1216));
		break;
	case 8:// Limit Break
		cur = (char *)data + 2756;
		hexEdit->setData(QByteArray(cur, 16));
		break;
	case 9:// Item
		cur = (char *)data + 2772;
		hexEdit->setData(QByteArray(cur, 428));
		break;
	case 10:// Config
		cur = (char *)data + 2704;
		hexEdit->setData(QByteArray(cur, 20));
		break;*/
	default:// All
		hexEdit->setData(QByteArray(/*(char *)descData, sizeof(*descData)).append(*/(char *)data, sizeof(*data)));
		break;
	}
}

void AllEditor::savePage()
{
	const char *hexdata = hexEdit->data().constData();
	char *cur;

	switch(tabBar->currentIndex()) {
//	case 1:// Header
//		memcpy(descData, hexdata, sizeof(*descData));
//		break;
//	case 1:// Misc 1
//		cur = (char *)data + 2724;
//		memcpy(cur, hexdata, 32);
//		break;
	case 1:// Misc 2/Position
		cur = (char *)data + 3308;
		memcpy(cur, hexdata, 36);
		break;
	case 2:// Shops
		cur = (char *)data + 2304;
		memcpy(cur, hexdata, 400);
		break;
	case 3:// Misc
		cur = (char *)data + 3344;
		memcpy(cur, hexdata, 256);
		break;
	case 4:// Field Vars
		cur = (char *)data + 3600;
		memcpy(cur, hexdata, 1024);
		break;
	case 5:// WorldMap Vars
		cur = (char *)data + 4624;
		memcpy(cur, hexdata, 128);
		break;
	case 6:// Chocobo World
		cur = (char *)data + 4880;
		memcpy(cur, hexdata, 64);
		break;
	/*case 6:// GF
		memcpy(data, hexdata, 1088);
		break;
	case 7:// Characters
		cur = (char *)data + 1088;
		memcpy(cur, hexdata, 1216);
		break;
	case 8:// Limit Break
		cur = (char *)data + 2756;
		memcpy(cur, hexdata, 16);
		break;
	case 9:// Item
		cur = (char *)data + 2772;
		memcpy(cur, hexdata, 428);
		break;
	case 10:// Config
		cur = (char *)data + 2704;
		memcpy(&data[2704], hexdata, 20);
		break;*/
	default:// All
//		memcpy(descData, hexdata, sizeof(*descData));
//		hexdata += sizeof(*descData);
		memcpy(data, hexdata, sizeof(*data));
		break;
	}
}
