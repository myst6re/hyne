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

#include "SaveData.h"

SaveData::SaveData()
	: _freqValue(60), _id(0), _isFF8(false), _isDelete(false), _isTheLastEdited(false), _isVmp(false), _isModified(false)
{
}

SaveData::SaveData(int id, const QByteArray &data, const QByteArray &MCHeader, bool isVmp)
	: _freqValue(60), _id(id), _isFF8(false), _isDelete(false), _isTheLastEdited(false), _isVmp(isVmp), _isModified(false)
{
	open(data, MCHeader);
}

void SaveData::open(const QByteArray &data, const QByteArray &MCHeader)
{
	_isDelete = _isFF8 = false;

	setMCHeader(MCHeader);
	if(data.isEmpty())	_isDelete = true;

	if(data.size() >= FF8SAVE_SIZE && data.startsWith("SC"))
	{
		_isFF8 = setData(data);

		if(_isFF8 || !_isDelete) {
			_header = data.left(96);
			switch((quint8)data.at(2)) {
			case 0x11:
				_icon.setAll(data.mid(96,_isFF8 ? 288 : 160));
				break;
			case 0x12:
				_icon.setAll(data.mid(96,288), 2);
				break;
			case 0x13:
				_icon.setAll(data.mid(96,416), 3);
				break;
			}
		}
	}
	else
		_isDelete = true;

	if(!_isFF8) {
		_saveData = !_isDelete ? data.mid(96+_icon.save().size()) : data;
	}

	_isModified = false;
}

void SaveData::remove()
{
	open(QByteArray(), hasMCHeader() ? emptyMCHeader() : QByteArray());
	_isModified = true;
}

const QByteArray &SaveData::MCHeader() const
{
	return _MCHeader;
}

char SaveData::MCHeaderCountry() const
{
	return _MCHeader.at(11);
}

QString SaveData::MCHeaderCode() const
{
	return _MCHeader.mid(12, 10);
}

QString SaveData::MCHeaderId() const
{
	return _MCHeader.mid(22, 8);
}

QByteArray SaveData::saveMCHeader()
{
	if(hasMCHeader()) {
		_MCHeader = _MCHeader.leftJustified(127, '\x00', true);
		return _MCHeader + (char)xorByte(_MCHeader.constData());
	}
	else {
		return emptyMCHeader();
	}
}

QByteArray SaveData::emptyMCHeader()
{
	return QByteArray("\xa1\x00\x00\x00\x00\x00\x00\x00\xff\xff", 10).append(QByteArray(118, '\x00'));
}

void SaveData::setMCHeader(const QByteArray &MCHeader)
{
	if(_MCHeader != MCHeader) {
		_isModified = true;
		_MCHeader = MCHeader;
	}

	if(hasMCHeader()) {
		if(!_isVmp) {
			_isDelete = (quint8)MCHeader.at(0) >> 4 != 0x5;// 0xa1-0xa2-0xa3 : deleted | 0xa0 : formated | 0x51 : has data
		}else
			_isDelete = false;
		_freqValue = MCHeaderCountry()==COUNTRY_EU ? 50 : 60;
	}
	else {
		_isDelete = false;
		_freqValue = 60;
	}
}

void SaveData::setMCHeader(bool exists, char country, const QString &code, const QString &id)
{
	QByteArray id8, code10;

	if(id.isEmpty()) {
		if(country == COUNTRY_EU)
			id8 = "0520";
		else if(country == COUNTRY_US)
			id8 = "0426";
		else
			id8 = "FF08";
		id8.append(QString("%1").arg(this->id(), 2, 10, QChar('0')).toLatin1());
	}
	else {
		id8 = id.toLatin1();
	}

	code10 = code.toLatin1().leftJustified(10, '\0', true);
	id8 = id8.leftJustified(8, '\0', true);

	if(_MCHeader.isEmpty()) {
		_MCHeader.append(exists ? 0x51 : 0xa1);//1
		_MCHeader.append("\x00\x00\x00\x00\x20\x00\x00\xff\xff", 9);//Main header (9)
		_MCHeader.append('B');//1
		_MCHeader.append(country);//1
		_MCHeader.append(code10);//10
		_MCHeader.append(id8);//8
	} else {
		_MCHeader[0] = exists ? 0x51 : 0xa1;
		_MCHeader[10] = 'B';
		_MCHeader[11] = country;
		_MCHeader.replace(12, 10, code10);
		_MCHeader.replace(22, 8, id8);
	}

	setMCHeader(_MCHeader);// update infos
}

const QByteArray &SaveData::header() const
{
	return _header;
}

QPixmap SaveData::icon(bool chocobo_world_icon) const
{
	return _icon.icon(chocobo_world_icon);
}

const SaveIcon *SaveData::saveIcon() const
{
	return &_icon;
}

const HEADER &SaveData::descData() const
{
	return _descData;
}

const MAIN &SaveData::mainData() const
{
	return _mainData;
}

void SaveData::setSaveData(const HEADER &descData, const MAIN &data)
{
	if(memcmp(&descData, &_descData, sizeof(HEADER)) != 0 || memcmp(&data, &_mainData, sizeof(MAIN)) != 0) {
		_isModified = true;
		_descData = descData;
		_mainData = data;

		// Update descData

		quint8 leader;
		quint8 perso1 = _mainData.misc1.party[0];
		quint8 perso2 = _mainData.misc1.party[1];
		quint8 perso3 = _mainData.misc1.party[2];

		if(perso1 != 0xFF && perso1 < 8)
			leader = perso1;
		else if(perso2 != 0xFF && perso2 < 8)
			leader = perso2;
		else if(perso3 != 0xFF && perso3 < 8)
			leader = perso3;
		else
			leader = 0;
		if(perso1 != 0xFF && perso1 < 8)	perso1 = _mainData.persos[perso1].ID;
		if(perso2 != 0xFF && perso2 < 8)	perso2 = _mainData.persos[perso2].ID;
		if(perso3 != 0xFF && perso3 < 8)	perso3 = _mainData.persos[perso3].ID;

		_descData.party[0] = perso1;
		_descData.party[1] = perso2;
		_descData.party[2] = perso3;

		_descData.nivLeader = _mainData.persos[leader].exp/1000 + 1;
		_descData.hpLeader = _mainData.persos[leader].current_HPs;
		_descData.gils = _mainData.misc2.dream & 1 ? _mainData.misc1.dream_gils : _mainData.misc1.gils;
		_descData.time = _mainData.misc2.game_time;
	}
}

bool SaveData::isModified() const
{
	return _isModified;
}

void SaveData::setModified(bool modified)
{
	_isModified = modified;
}

int SaveData::freqValue() const
{
	return _freqValue;
}

int SaveData::id() const
{
	return _id;
}

void SaveData::setId(int id)
{
	_id = id;
}

bool SaveData::isFF8() const
{
	return _isFF8;
}

bool SaveData::isDelete() const
{
	return _isDelete;
}

void SaveData::setIsTheLastEdited(bool isTheLast)
{
	_isTheLastEdited = isTheLast;
}

bool SaveData::isTheLastEdited() const
{
	return _isTheLastEdited;
}

bool SaveData::hasMCHeader() const
{
	return !_MCHeader.isEmpty();
}

bool SaveData::exportPC(const QString &path) const
{
	QFile fic(path);
	if(!fic.open(QIODevice::WriteOnly))	return false;
	
	QByteArray data = LZS::compress(save());
	quint32 size = data.size();
	
	fic.write((char *)&size, 4);
	fic.write(data);
	fic.close();

	return true;
}

void SaveData::restore()
{
	_isModified = true;
	_isDelete = false;
	_MCHeader.replace(0, 10, QByteArray("\x51\x00\x00\x00\x00\x20\x00\x00\xff\xff", 10));
	_MCHeader.replace(28, 99, QByteArray(99, '\x00'));
}

QString SaveData::getShortDescription() const
{
	QTextCodec *codec = QTextCodec::codecForName(QByteArray("Shift-JIS"));
	if(!_header.isEmpty() && codec!=0) {
		QByteArray desc_array = _header.mid(4,64);
		int index;
		if((index = desc_array.indexOf('\x00')) != -1) {
			desc_array.truncate(index);
		}
		return codec->toUnicode(desc_array);
	}
	return QString();
}

bool SaveData::setData(const QByteArray &data)
{
	const char *access_data = data.constData();
	//ff8
	quint16 ff8;
	memcpy(&ff8, &access_data[386], 2);
	if(ff8 != 0x8FF)		return false;

	if(sizeof(_descData)!=76) {
		qWarning() << "error desc structure" << sizeof(_descData);
		return false;
	}

	memcpy(&_descData, &access_data[388], sizeof(_descData));

	if(sizeof(_mainData)!=4944) {
		qWarning() << "error main structure" << sizeof(_mainData);
		return false;
	}

	memcpy(&_mainData, &access_data[464], sizeof(_mainData));

	return true;
}

QByteArray SaveData::save() const
{
	QByteArray ret;

	if(!_isFF8) {
		ret.append(_header).append(_icon.save()).append(_saveData);
		return ret.leftJustified(SAVE_SIZE, '\x00', true);
	}

	quint16 checksum = calcChecksum((char *)&_mainData);//On calcule le checksum à partir de la partie gf

	ret.append("SC", 2);
	ret.append(_header.at(2));// icon frames
	ret.append('\x01');// slot count
	ret.append("\x82\x65\x82\x65\x82\x57\x81\x6D", 8);// FF8[
	ret.append(FF8Text::numToBiosText(_id+1, 2));// II
	ret.append("\x81\x6E\x81\x5E", 4);// ]/
	ret.append(FF8Text::numToBiosText(Config::hour(_mainData.misc2.game_time, _freqValue), 2));// HH
	ret.append("\x81\x46", 2);// :
	ret.append(FF8Text::numToBiosText(Config::min(_mainData.misc2.game_time, _freqValue), 2));// MM
	ret.append(_header.right(66));
	ret.append(_icon.save());
	ret.append((char *)&checksum, 2);
	ret.append("\xFF\x08", 2);
	ret.append((char *)&_descData, sizeof(_descData));
	ret.append((char *)&_mainData, sizeof(_mainData));
	ret.append((char *)&checksum, 2);
	ret.append(QByteArray(2782,'\x00'));

	if(ret.size()!=SAVE_SIZE)	qWarning() << "Error saved save size" << ret.size() << SAVE_SIZE;
	
	return ret;
}

bool SaveData::isJp() const
{
	return hasMCHeader() ? MCHeaderCountry()==COUNTRY_JP : false;
}

bool SaveData::isVmp() const
{
	return _isVmp;
}

QString SaveData::perso(quint8 index) const
{
	switch(index) {
	case SQUALL:		return FF8Text::toString(_descData.squall, isJp());
	case RINOA:			return FF8Text::toString(_descData.linoa, isJp());
	case GRIEVER:		return FF8Text::toString(_mainData.misc1.griever, isJp());
	case BOKO:			return FF8Text::toString(_descData.boko, isJp());
	case ANGELO:		return FF8Text::toString(_descData.angelo, isJp());
	default:
		if(index < 16)	return Data::names.at(index);
		return QString();
	}
}

quint8 SaveData::xorByte(const char *data)
{
	register quint8 xorByte=0;
	for(int i=0 ; i<127 ; ++i)
		xorByte ^= data[i];
	return xorByte;
}

quint16 SaveData::calcChecksum(const char *data)
{
	register quint16 crc = 0xFFFF, cur;

	for(cur=0 ; cur<4944 ; ++cur)
		crc = crcTab[(crc >> 8) ^ (quint8)*data++] ^ (crc << 8);

	return ~crc;
}

const quint16 SaveData::crcTab[256] = {0,4129,8258,12387,16516,20645,24774,28903,33032,37161,41290,45419,49548,53677,57806,61935,4657,528,12915,8786,21173,17044,29431,25302,37689,33560,45947,41818,54205,50076,62463,58334,9314,13379,1056,5121,25830,29895,17572,21637,42346,46411,34088,38153,58862,62927,50604,54669,13907,9842,5649,1584,30423,26358,22165,18100,46939,42874,38681,34616,63455,59390,55197,51132,18628,22757,26758,30887,2112,6241,10242,14371,51660,55789,59790,63919,35144,39273,43274,47403,23285,19156,31415,27286,6769,2640,14899,10770,56317,52188,64447,60318,39801,35672,47931,43802,27814,31879,19684,23749,11298,15363,3168,7233,60846,64911,52716,56781,44330,48395,36200,40265,32407,28342,24277,20212,15891,11826,7761,3696,65439,61374,57309,53244,48923,44858,40793,36728,37256,33193,45514,41451,53516,49453,61774,57711,4224,161,12482,8419,20484,16421,28742,24679,33721,37784,41979,46042,49981,54044,58239,62302,689,4752,8947,13010,16949,21012,25207,29270,46570,42443,38312,34185,62830,58703,54572,50445,13538,9411,5280,1153,29798,25671,21540,17413,42971,47098,34713,38840,59231,63358,50973,55100,9939,14066,1681,5808,26199,30326,17941,22068,55628,51565,63758,59695,39368,35305,47498,43435,22596,18533,30726,26663,6336,2273,14466,10403,52093,56156,60223,64286,35833,39896,43963,48026,19061,23124,27191,31254,2801,6864,10931,14994,64814,60687,56684,52557,48554,44427,40424,36297,31782,27655,23652,19525,15522,11395,7392,3265,61215,65342,53085,57212,44955,49082,36825,40952,28183,32310,20053,24180,11923,16050,3793};
