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

#include "SaveData.h"
#include "FF8Text.h"
#include "Data.h"
#include "LZS.h"

SaveData::SaveData() :
	_freqValue(60), _id(0), _isFF8(false), _isDelete(false),
	_isTheLastEdited(false), _hasExistsInfos(true), _isRaw(false),
	_isModified(false), _wasModified(false), _descriptionAuto(true), _previewAuto(true)
{
}

SaveData::SaveData(int id, const QByteArray &data, const QByteArray &MCHeader, bool hasExistsInfos, bool isRaw) :
	_freqValue(60), _id(id), _isFF8(false), _isDelete(false),
	_isTheLastEdited(false), _hasExistsInfos(hasExistsInfos), _isRaw(isRaw),
	_isModified(false), _wasModified(false), _descriptionAuto(true), _previewAuto(true)
{
	open(data, MCHeader);
}

void SaveData::open(const QByteArray &data, const QByteArray &MCHeader)
{
	_isModified = _isDelete = _isFF8 = false;
	_header.clear();
	_icon.setData(QByteArray());
	_saveData.clear();

	_MCHeader = MCHeader;
	setMCHeader(MCHeader);
	if(data.isEmpty())	_isDelete = true;

	if(!_isRaw) {
		if(data.size() >= FF8SAVE_SIZE && data.startsWith("SC")) {
			_isFF8 = setData(data);

			if(_isFF8 || !_isDelete) {
				_header = data.left(96);
				_icon.setData(data.mid(96, 416));

				switch((quint8)data.at(2)) {
				case 0x11:
					_icon.setNbFrames(1);
					break;
				case 0x12:
					_icon.setNbFrames(2);
					break;
				case 0x13:
					_icon.setNbFrames(3);
					break;
				}
			}
		} else {
			_isDelete = true;
		}
	}

	if(!_isFF8) {
		_saveData = hasSCHeader() ? data.mid(512) : data;
	}
}

QByteArray SaveData::save() const
{
	QByteArray ret;

	if(!_isFF8) {
		ret.append(_header).append(_icon.data()).append(_saveData);
		return ret.leftJustified(SAVE_SIZE, '\x00', true);
	}

	quint16 checksum = calcChecksum((char *)&_mainData);//On calcule le checksum à partir de la partie gf

	ret.append("SC", 2);
	ret.append(_header.at(2));// icon frames
	ret.append('\x01');// slot count
	if(_descriptionAuto) {
		ret.append("\x82\x65\x82\x65\x82\x57\x81\x6D", 8);// FF8[
		ret.append(FF8Text::numToBiosText(_id+1, 2));// II
		ret.append("\x81\x6E\x81\x5E", 4);// ]/
		ret.append(FF8Text::numToBiosText(Config::hour(_mainData.misc2.game_time, _freqValue), 2));// HH
		ret.append("\x81\x46", 2);// :
		ret.append(FF8Text::numToBiosText(Config::min(_mainData.misc2.game_time, _freqValue), 2));// MM
		ret.append(_header.right(66));
	} else {
		ret.append(_header.right(92));
	}
	ret.append(_icon.data().leftJustified(288, '\0', true));
	ret.append((char *)&checksum, 2);
	ret.append("\xFF\x08", 2);
	ret.append((char *)&_descData, sizeof(_descData));
	ret.append((char *)&_mainData, sizeof(_mainData));
	ret.append((char *)&checksum, 2);
	ret.append(QByteArray(2782,'\x00'));

	if(ret.size() != SAVE_SIZE) {
		ret = ret.leftJustified(SAVE_SIZE, '\x00', true);
		qWarning() << "Error saved save size" << ret.size() << SAVE_SIZE;
		Q_ASSERT(false);
	}

	return ret;
}

void SaveData::remove()
{
	open(QByteArray(), hasMCHeader() ? emptyMCHeader() : QByteArray());
	_isModified = true;
}

void SaveData::restore()
{
	_isModified = true;
	_isDelete = false;
	_MCHeader.replace(0, 10, QByteArray("\x51\x00\x00\x00\x00\x20\x00\x00\xff\xff", 10));
	_MCHeader.replace(28, 99, QByteArray(99, '\x00'));
}

bool SaveData::hasMCHeader() const
{
	return !_MCHeader.isEmpty();
}

const QByteArray &SaveData::MCHeader() const
{
	return _MCHeader;
}

char SaveData::MCHeaderCountry() const
{
	return _MCHeader.at(11);
}

bool SaveData::isJp() const
{
	return hasMCHeader() ? MCHeaderCountry()==COUNTRY_JP : false;
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
	return QByteArray("\xa1\x00\x00\x00\x00\x00\x00\x00\xff\xff", 10)
			.append(QByteArray(118, '\x00'));
}

void SaveData::setMCHeader(const QByteArray &MCHeader)
{
	if(_MCHeader != MCHeader) {
		_isModified = true;
		_MCHeader = MCHeader;
	}

	if(hasMCHeader()) {
		_isDelete = (quint8)MCHeader.at(0) >> 4 != 0x5;// 0xa1-0xa2-0xa3 : deleted | 0xa0 : formated | 0x51 : has data
		_freqValue = MCHeaderCountry()==COUNTRY_EU ? 50 : 60;
	}
	else {
		_isDelete = false;
		_freqValue = 60;
	}
}

void SaveData::setMCHeader(bool exists, char country, const QString &code, const QString &id)
{
	QByteArray id8, code10, MCHeader = _MCHeader;

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

	if(MCHeader.isEmpty()) {
		MCHeader.append(exists ? '\x51' : '\xa1');//1
		MCHeader.append("\x00\x00\x00\x00\x20\x00\x00\xff\xff", 9);//Main header (9)
		MCHeader.append('B');//1
		MCHeader.append(country);//1
		MCHeader.append(code10);//10
		MCHeader.append(id8);//8
	} else {
		MCHeader[0] = exists ? '\x51' : '\xa1';
		MCHeader[10] = 'B';
		MCHeader[11] = country;
		MCHeader.replace(12, 10, code10);
		MCHeader.replace(22, 8, id8);
	}

	setMCHeader(MCHeader);// update infos
}

HEADER &SaveData::descData()
{
	return _descData;
}

const HEADER &SaveData::constDescData() const
{
	return _descData;
}

MAIN &SaveData::mainData()
{
	return _mainData;
}

const MAIN &SaveData::constMainData() const
{
	return _mainData;
}

void SaveData::updateDescData()
{
	if(_previewAuto) {
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
		_descData.gils = (_mainData.misc2.dream & 1) ? _mainData.misc1.dream_gils
		                                             : _mainData.misc1.gils;
		_descData.time = _mainData.misc2.game_time;
		_descData.disc = _mainData.misc3.disc - 1;

		_isModified = true;
	}
}

void SaveData::setSaveData(const HEADER &descData, const MAIN &data)
{
	if(memcmp(&descData, &_descData, sizeof(HEADER)) != 0 || memcmp(&data, &_mainData, sizeof(MAIN)) != 0) {
		_isModified = true;
		_descData = descData;
		_mainData = data;
		updateDescData();
	}
}

bool SaveData::isModified() const
{
	return _isModified;
}

bool SaveData::wasModified() const
{
	return _wasModified;
}

void SaveData::setModified(bool modified)
{
	if(_isModified && !modified) {
		_wasModified = true;
	}
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

bool SaveData::hasExistsInfos() const
{
	return _hasExistsInfos;
}

bool SaveData::isRaw() const
{
	return _isRaw;
}

bool SaveData::hasSCHeader() const
{
	return !_header.isEmpty();
}

quint8 SaveData::blockCount() const
{
	return hasSCHeader() ? qMax(quint8(_header.at(3)), quint8(1)) : 1;
}

void SaveData::setBlockCount(quint8 blockCount)
{
	if(hasSCHeader()) {
		_header[3] = blockCount;
		_isModified = true;
	}
}

QString SaveData::shortDescription() const
{
	if(hasSCHeader()) {
		try {
			QTextCodec *codec = QTextCodec::codecForName("Shift-JIS");
			if(codec) {
				QByteArray desc_array = _header.mid(4, 64);
				int index;
				if((index = desc_array.indexOf('\x00')) != -1) {
					desc_array.truncate(index);
				}
				return codec->toUnicode(desc_array);
			}
		} catch(...) {
		}
	}
	return QString();
}

void SaveData::setShortDescription(const QString &desc)
{
	if(hasSCHeader()) {
		QByteArray desc_data;

		if(!desc.isEmpty()) {
			try {
				QTextCodec *codec = QTextCodec::codecForName("Shift-JIS");
				if(codec) {
					desc_data = codec->fromUnicode(desc);
				} else {
					return;
				}
			} catch(...) {
				return;
			}
		}

		desc_data = desc_data.leftJustified(64, '\0', true);

		if(_header.mid(4, 64) != desc_data) {
			_header.replace(4, 64, desc_data);
			_isModified = true;
		}
	}
}

bool SaveData::isDescriptionAuto() const
{
	return isFF8() && _descriptionAuto;
}

void SaveData::setDescriptionAuto(bool descAuto)
{
	_descriptionAuto = descAuto;
}

const SaveIconData &SaveData::saveIcon() const
{
	return _icon;
}

void SaveData::setSaveIcon(const SaveIconData &saveIconData)
{
	if(_icon.data() != saveIconData.data()) {
		_icon = saveIconData;
		_isModified = true;
	}
}

bool SaveData::isFF8() const
{
	return _isFF8;
}

bool SaveData::setData(const QByteArray &data)
{
	const char *access_data = data.constData();
	//ff8
	quint16 ff8;
	memcpy(&ff8, &access_data[386], 2);
	// PC Demo is 0xFF8
	if(ff8 != 0x8FF && ff8 != 0xFF8)		return false;

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

QString SaveData::perso(quint8 index) const
{
	switch(index) {
	case SQUALL:		return FF8Text::toString((char *)_descData.squall, isJp());
	case RINOA:			return FF8Text::toString((char *)_descData.rinoa, isJp());
	case GRIEVER:		return FF8Text::toString((char *)_mainData.misc1.griever, isJp());
	case BOKO:			return FF8Text::toString((char *)_descData.boko, isJp());
	case ANGELO:		return FF8Text::toString((char *)_descData.angelo, isJp());
	default:
		if(index < 16)	return Data::names().at(index);
		return QString();
	}
}

QString SaveData::gf(quint8 index) const
{
	return FF8Text::toString((char *)_mainData.gfs[index].name, isJp());
}

void SaveData::setGf(quint8 index, const QString &name)
{
	const char *gfName = FF8Text::toByteArray(name, isJp())
			.leftJustified(11, '\x00', true)
			.append('\x00').constData();

	memcpy(_mainData.gfs[index].name, gfName, 12);
}

void SaveData::setPerso(quint8 index, const QString &name)
{
	const char *persoName = FF8Text::toByteArray(name, isJp())
			.leftJustified(11, '\x00', true)
			.append('\x00').constData();

	switch(index) {
	case SQUALL:		memcpy(_descData.squall, persoName, 12);		break;
	case RINOA:			memcpy(_descData.rinoa, persoName, 12);			break;
	case GRIEVER:		memcpy(_mainData.misc1.griever, persoName, 12);	break;
	case BOKO:			memcpy(_descData.boko, persoName, 12);			break;
	case ANGELO:		memcpy(_descData.angelo, persoName, 12);		break;
	default:	break;
	}
}

bool SaveData::isPreviewAuto() const
{
	return isFF8() && _previewAuto;
}

void SaveData::setPreviewAuto(bool prevAuto)
{
	_previewAuto = prevAuto;
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
