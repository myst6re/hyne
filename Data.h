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

#define MAP_COUNT	982

#include <QtCore>

typedef struct CoupleInt {
	CoupleInt() : one(0), two(0) {}
	CoupleInt(int one, int two) : one(one), two(two) {}
	int one, two;
} cInt;

class DataList
{
public:
	DataList() {}
	const QStringList &list() {
		if(_list.isEmpty())		fillList();
		return _list;
	}
	const QString &at(int id) {
		if(_list.isEmpty())		fillList();
		return _list.at(id);
	}
	QString value(int id, const QString &defaultValue=QString()) {
		if(_list.isEmpty())		fillList();
		return _list.value(id, defaultValue);
	}
	int size() {
		if(_list.isEmpty())		fillList();
		return _list.size();
	}
	inline void clear() {
		_list.clear();
	}
protected:
	virtual void fillList()=0;
	QStringList _list;
};

class Abilities : public DataList
{
public:
	Abilities() {}
protected:
	virtual void fillList();
};

class Magics : public DataList
{
public:
	Magics() {}
protected:
	virtual void fillList();
};

class Items : public DataList
{
public:
	Items() {}
protected:
	virtual void fillList();
};

class Locations : public DataList
{
public:
	Locations() {}
protected:
	virtual void fillList();
};

class Weapons : public DataList
{
public:
	Weapons() {}
protected:
	virtual void fillList();
};

class Names : public DataList
{
public:
	Names() {}
protected:
	virtual void fillList();
};

class GfNames : public DataList
{
public:
	GfNames() {}
protected:
	virtual void fillList();
};

class Cards : public DataList
{
public:
	Cards() {}
protected:
	virtual void fillList();
};

class ZellLBs : public DataList
{
public:
	ZellLBs() {}
protected:
	virtual void fillList();
};

class IrvineLBs : public DataList
{
public:
	IrvineLBs() {}
protected:
	virtual void fillList();
};

class QuistisLBs : public DataList
{
public:
	QuistisLBs() {}
protected:
	virtual void fillList();
};

class RinoaLBs : public DataList
{
public:
	RinoaLBs() {}
protected:
	virtual void fillList();
};

class Ennemies : public DataList
{
public:
	Ennemies() {}
	void setNames(Names *names) {
		_names = names;
	}
protected:
	virtual void fillList();
private:
	Names *_names;
};

class Data
{
public:
	static const quint8 apsTab[116];
	static const quint8 innateAbilities[16][22];
	static QString getCardsLocation(int i);
	static const int cardsLocation[256];
	static const cInt momentLocation[403];
	static const int drawPoints[256];
	static const int drawPointsLoc[256];

	static void reload();

	static inline Abilities &abilities() { return _abilities; }
	static inline Magics &magics() { return _magic; }
	static inline Items &items() { return _items; }
	static inline Locations &locations() { return _locations; }
	static inline Weapons &weapons() { return _weapons; }
	static inline Names &names() { return _names; }
	static inline GfNames &gfnames() { return _gfnames; }
	static inline Cards &cards() { return _cards; }
	static inline ZellLBs &zellLBs() { return _zellLB; }
	static inline IrvineLBs &irvineLBs() { return _irvineLB; }
	static inline QuistisLBs &quistisLBs() { return _quistisLB; }
	static inline RinoaLBs &rinoaLBs() { return _rinoaLB; }
	static inline Ennemies &ennemies() { _ennemies.setNames(&_names); return _ennemies; }

	static qint8 abilityType(quint8 abilityID);
	static quint8 itemType(quint8 itemID);

	static QStringList maplist();
private:
	static void fillAbilities();
	static Abilities _abilities;
	static Magics _magic;
	static Items _items;
	static Locations _locations;
	static Weapons _weapons;
	static Names _names;
	static GfNames _gfnames;
	static Cards _cards;
	static ZellLBs _zellLB;
	static IrvineLBs _irvineLB;
	static QuistisLBs _quistisLB;
	static RinoaLBs _rinoaLB;
	static Ennemies _ennemies;
	static const char *_maplist[MAP_COUNT];
};

#endif
