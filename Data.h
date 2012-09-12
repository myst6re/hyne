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

#ifndef DEF_DATA
#define DEF_DATA

#define SQUALL		0
#define ZELL		1
#define IRVINE		2
#define QUISTIS		3
#define RINOA		4
#define SELPHIE		5
#define SEIFER		6
#define EDEA		7
#define LAGUNA		8
#define KIROS		9
#define WARD		10
#define GRIEVER		12
#define BOKO		14
#define ANGELO		15

#include <QtCore>
#include <QComboBox>

typedef struct CoupleInt {
	CoupleInt() : one(0), two(0) {}
	CoupleInt(int one, int two) : one(one), two(two) {}
	int one, two;
} cInt;

class Data
{
public:
	static void load();

	static const quint8 apsTab[116];
	static const quint8 innateAbilities[16][22];
	static QString getCardsLocation(int i);
	static const int cardsLocation[256];
	static const cInt momentLocation[403];
	static const int drawPoints[256];
	static const int drawPointsLoc[256];
	static QStringList abilities;
	static QStringList magic;
	static QStringList items;
	static QStringList locations;
	static QStringList weapons;
	static QStringList names;
	static QStringList gfnames;
	static QStringList cards;
	static QStringList zellLB;
	static QStringList irvineLB;
	static QStringList quistisLB;
	static QStringList rinoaLB;
	static QStringList ennemies;

	static QMap<int, QIcon> abilityIcons();
	static void fillAbilities(QComboBox *comboBox, const QMap<int, QIcon> &icons);
	static qint8 abilityType(quint8 abilityID);
	static quint8 itemType(quint8 itemID);
};

#endif
