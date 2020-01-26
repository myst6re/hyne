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

#include "SavecardData.h"
#include "GZIP.h"
#include "Parameters.h"
#include "LZS.h"
#include "CryptographicHash.h"

SavecardData::SavecardData(const QString &path, quint8 slot, const FF8Installation &ff8Installation) :
	_ok(true), start(0), _isModified(false), _slot(slot), _ff8Installation(ff8Installation)
{
	open(path, slot);
}

SavecardData::SavecardData(int saveCount) :
	_ok(true), start(0), _isModified(false)
{
	for(int i=0 ; i<saveCount ; ++i) {
		addSave();
	}

	setType(Undefined);
}

SavecardData::~SavecardData()
{
	foreach(SaveData *save, saves)	delete save;
}

bool SavecardData::open(const QString &path, quint8 slot)
{
	if(slot)
	{
		setPath(QDir::fromNativeSeparators(QDir::cleanPath(path)) + "/");
		setType(PcSlot);

		directory(_ff8Installation.saveNamePattern(_slot));
		_ok = !saves.isEmpty();
	}
	else
	{
		setPath(QDir::fromNativeSeparators(QDir::cleanPath(path)));
		QString extension = this->extension();

		if(extension.isEmpty() || extension == "ff8")
		{
			setType(Pc);
			_ok = pc();
		}
		else if(extension == "mcr" || extension == "ddf" || extension == "mc"
		   || extension == "mcd"|| extension == "mci" || extension == "ps"
		   || extension == "psm" || extension == "vm1" || extension =="srm")
		{
			setType(Ps);
			_ok = ps();
		}
		else if(extension == "mem" || extension == "vgs")
		{
			setType(Vgs);
			_ok = ps();
		}
		else if(extension == "gme")
		{
			setType(Gme);
			_ok = ps();
		}
		else if(extension == "vmp")
		{
			setType(Vmp);
			_ok = ps();
		}
		else if(extension == "psv")
		{
			setType(Psv);
			_ok = ps3();
			if(!_ok) {
				_ok = sstate_pSX();
				setType(Undefined);
			}
		}
		else if(extension == "000"
				|| extension == "001"
				|| extension == "002"
				|| extension == "003"
				|| extension == "004")
		{
			_ok = sstate_ePSXe();
			setType(Undefined);
		}
		else
		{
			_ok = false;
			setType(Unknown);
		}
	}

	// compare(2, 3);

	return _ok;
}

#ifndef Q_OS_WINRT
const QFileSystemWatcher *SavecardData::watcher() const
{
	return &fileWatcher;
}
#endif

QString SavecardData::description() const
{
	return QString(_description);
}

void SavecardData::setDescription(const QString &desc)
{
	_description = desc.toLatin1();
}

const QByteArray &SavecardData::hashSeed() const
{
	return _hashSeed;
}

void SavecardData::setHashSeed(const QByteArray &hashSeed)
{
	_hashSeed = hashSeed;
}

QString SavecardData::dirname() const
{
	int index = _path.lastIndexOf('/');
	return index != -1 ? _path.left(index + 1) : QString();
}

QString SavecardData::name() const
{
	return _path.mid(_path.lastIndexOf('/') + 1);
}

QString SavecardData::nameNoExtension() const
{
	QString n = name();
	int index = n.lastIndexOf('.');
	if (index < 0) {
		return n;
	}
	return n.left(index);
}

QString SavecardData::extension() const
{
	int index = _path.lastIndexOf('.');
	return index != -1 ? _path.mid(index + 1).toLower() : QString();
}

void SavecardData::setName(const QString &name)
{
	_path = dirname() + name;
}

void SavecardData::setType(Type type)
{
	switch(type) {
	case Vgs:
		start = 64;
		break;
	case Gme:
		start = 3904;
		break;
	case Vmp:
		start = 128;
		break;
	case Undefined:
		start = 0;
		setPath(QObject::tr("Sans nom"));
		setModified(true);
		break;
	default:
		start = 0;
		break;
	}

	_type = type;
}

bool SavecardData::hasPath() const
{
	return !dirname().isEmpty();
}

bool SavecardData::isModified() const
{
	if(_isModified)	return true;

	foreach(SaveData *save, saves) {
		if(save->isModified())	return true;
	}
	return false;
}

void SavecardData::setModified(bool modified)
{
	_isModified = modified;

	if(modified == false) {
		foreach(SaveData *save, saves) {
			save->setModified(false);
		}
	}
}

void SavecardData::setIsTheLastEdited(int saveID)
{
	foreach(SaveData *save, saves) {
		if(save->isTheLastEdited()) {
			save->setIsTheLastEdited(false);
		}
	}
	saves.at(saveID)->setIsTheLastEdited(true);
}

bool SavecardData::ps()
{
	setErrorString(QString());
	QFile fic(_path);

	if(!fic.exists())
	{
		setErrorString(QObject::tr("Le fichier n'existe plus.\n%1").arg(_path));
		return false;
	}
	if(!fic.open(QIODevice::ReadOnly))
	{
		setErrorString(QObject::tr("Le fichier est protégé en lecture."));
		return false;
	}
	if(fic.size() < start+131072)//start+8192*16
	{
		setErrorString(QObject::tr("Fichier trop court"));
		return false;
	}

	if(_type == Gme) {
		fic.seek(64);
		_description = fic.read(3840);
	} else if(_type == Vmp) {
		fic.seek(12);
		_hashSeed = fic.read(20);
	}

	fic.seek(start+128);
	QByteArray header = fic.read(1920);//(128*15)
	fic.seek(start+SAVE_SIZE);

	quint8 blockCount = 1;
	bool occupied;

	for(quint8 i=0 ; i<15 ; ++i) {
		if(blockCount <= 1 && !saves.isEmpty()) {
			blockCount = saves.last()->blockCount();
		}

		if(blockCount > 1) {
			blockCount--;
			occupied = true;
		} else {
			occupied = false;
		}

		addSave(fic.read(SAVE_SIZE), header.mid(128*i, 127), occupied);
	}

#ifndef Q_OS_WINRT
	if(fileWatcher.files().size()<30)
		fileWatcher.addPath(_path);
#endif

	return true;
}

bool SavecardData::ps3()
{
	QFile fic(_path);
	setErrorString(QString());

	if(!fic.exists())
	{
		setErrorString(QObject::tr("Le fichier n'existe plus.\n%1").arg(_path));
		return false;
	}
	if(!fic.open(QIODevice::ReadOnly))
	{
		setErrorString(QObject::tr("Le fichier est protégé en lecture."));
		return false;
	}

	// Check format
	fic.seek(1);
	if(QString(fic.peek(3)) != "VSP" && QString(fic.peek(6)) == "RS2CPU")
		return false;

	if(fic.size() < 8324)
	{
		setErrorString(QObject::tr("Fichier invalide"));
		return false;
	}

	fic.seek(8);
	QByteArray seed = fic.read(20);

	fic.seek(100);
	QByteArray MCHeader("\x51\x00\x00\x00\x00\x20\x00\x00\xff\xff", 10);
	MCHeader.append(fic.read(32));

	fic.seek(132);

	addSave(fic.read(SAVE_SIZE), MCHeader);

	if(saves.first()->isDelete())	return false;

	_hashSeed = seed;

#ifndef Q_OS_WINRT
	if(fileWatcher.files().size()<30)
		fileWatcher.addPath(_path);
#endif

	return true;
}

bool SavecardData::pc(const QString &path)
{
	qint32 sizeC;
	QFile f(path.isEmpty() ? _path : path);

	if(!f.exists() || !f.open(QIODevice::ReadOnly) || f.size() > SAVE_SIZE * 8) {
		return false;
	}

	f.read((char *)&sizeC, 4);
	qint32 sizeC2;
	f.read((char *)&sizeC2, 4);

	if(sizeC == sizeC2 + 4) {
		setType(Switch);
		_switchSaveSize = f.size();
	} else {
		// Revert previous move
		f.seek(f.pos() - 4);
	}

	if(type() != Switch && sizeC != f.size()-4) {
		quint16 header = sizeC & 0xFFFF;
		if(header == 0x4353) { // SC
			f.reset();
			addSave(f.read(FF8SAVE_SIZE));

			setType(PcUncompressed);

			return true;
		}
	}

	addSave(LZS::decompress(f.readAll(), FF8SAVE_SIZE));

//#ifndef Q_OS_WINRT
//	if(fileWatcher.files().size()<30)
//		fileWatcher.addPath(_path);
//#endif

	return true;
}

bool SavecardData::getFormatFromRaw()
{
	QFile f(_path);
	start = 0;
	_ok = false;

	setErrorString(QString());

	if(!f.exists() || !f.open(QIODevice::ReadOnly)) {
		setErrorString(QObject::tr("Impossible d'ouvrir le fichier"));
		return false;
	}

	QByteArray data = f.read(10000);

	int indexMC = data.indexOf("MC");
	if(indexMC != -1) { // Maybe Memory Card format
		start = indexMC;
		switch(indexMC) {
		case 0:
			_type = Ps;
			break;
		case 64:
			_type = Vgs;
			break;
		case 128:
			_type = Vmp;
			break;
		case 3904:
			_type = Gme;
			break;
		default:
			setType(Undefined);
			start = indexMC;
			break;
		}
		_ok = ps();
		return _ok;
	} else { // Maybe PC format
		if(data.startsWith("SC")) {
			_ok = pc();
			return _ok;
		}

		// compressed?
		if (data.size() > 4) {
			qint32 compressedOffset = -1, lzsSize;
			memcpy(&lzsSize, data.constData(), 4);

			if(lzsSize + 4 == f.size()) {
				compressedOffset = 4;
			} else {
				quint32 lzsSize2;
				memcpy(&lzsSize2, data.constData() + 4, 4);

				if(lzsSize == lzsSize2 + 4) {
					compressedOffset = 8;
				}
			}

			if(compressedOffset >= 0) {
				_ok = pc();
				return _ok;
			}
		}

		if(data.indexOf("VSP") == 1) {
			_type = Psv;
			_ok = ps3();

			return _ok;
		}
	}

	setErrorString(QObject::tr("Rien trouvé"));

	return false;
}

bool SavecardData::sstate_ePSXe()
{
	QTemporaryFile temp;
	setErrorString(QString());

	if(!temp.open()) {
		setErrorString(QObject::tr("Impossible de créer le fichier temporaire."));
		return false;
	}
	if(!GZIP::decompress(_path, temp.fileName())) {
		setErrorString(QObject::tr("Impossible de décompresser le fichier."));
		return false;
	}
//	temp.seek(7);
//	QString serial(temp.read(11));

	temp.seek(0xBC42);
	QByteArray MCHeader = temp.read(0x20);

	temp.seek(0x779CC);

	return sstate(temp.read(0x139E), MCHeader);
}

bool SavecardData::sstate_pSX()
{
	QFile f(_path);
	setErrorString(QString());

	if(!f.open(QIODevice::ReadOnly)) {
		setErrorString(QObject::tr("Le fichier est protégé en lecture."));
		return false;
	}

	f.seek(0xBD38);
	QByteArray MCHeader = f.read(0x20);

	f.seek(0x77AC2);

	return sstate(f.read(0x139E), MCHeader);
}

bool SavecardData::sstate(const QByteArray &fdata, const QByteArray &MCHeader)
{
	QByteArray squallIcon;

	QFile iconFile(":/data/icon0.psico");
	if(iconFile.open(QIODevice::ReadOnly)) {
		squallIcon = iconFile.readAll();
		iconFile.close();
	}

	QByteArray data;
	data.append("SC");
	data.append('\x11'); // icon (1 frame)
	data.append('\x01'); // 1 save slot per save
	data.append(QByteArray(0x4C, '\0')); // generic desc
	data.append("\x01\x00CRD0", 6);
	data.append(QByteArray(0x0A, '\0'));
	data.append(squallIcon.leftJustified(288, '\0', true));
	data.append("\x00\x00", 2); // CRC
	data.append(fdata);

	if(data.size() != 0x139E + 0x182) {
		setErrorString(QObject::tr("Format invalide."));
		return false;
	}

	addSave(data, MCHeader);

	if(!saves.isEmpty()) {
		SaveData *saveData = saves.last();
		if(!saveData->isFF8()) {
			saves.removeLast();
			delete saveData;
			setErrorString(QObject::tr("La sauvegarde trouvée n'est pas de Final Fantasy VIII."));
			return false;
		}
	}

	return true;
}

void SavecardData::directory(const QString &filePattern)
{
	QString dirname = this->dirname();

	for(quint8 i=0 ; i<30 ; ++i) {
		QString path = filePattern;
		QString num = QString("%1").arg(i + 1, 2, 10, QChar('0'));
		path = dirname + path.replace("{num}", num);
		if(!pc(path)) {
			addSave(); // Empty save
		}
	}

	LZS::clear();
}

void SavecardData::addSave(const QByteArray &data, const QByteArray &header, bool occupied)
{
	saves.append(new SaveData(saves.size(), data, header, type()!=Psv, occupied));
}

void SavecardData::moveSave(int sourceID, int targetID)
{
	SaveData *saveData = saves.takeAt(sourceID);
	saves.insert(targetID, saveData);

	// Rebuild ids
	int saveID = 0;
	foreach(saveData, saves) {
		saveData->setId(saveID);
		saveID++;
	}

	setModified(true);
}

SaveData *SavecardData::getSave(int id) const
{
	return saves.value(id, nullptr);
}

int SavecardData::saveCount() const
{
	return saves.size();
}

bool SavecardData::saveMemoryCard(const QString &saveAs, Type newType)
{
	const QString path = saveAs.isEmpty() ? _path : saveAs;
	QTemporaryFile temp;
	QFile fic(_path);
	setErrorString(QString());

	if(saveAs.isEmpty()) {
		newType = _type;
	}

	if(!fic.exists())
	{
		setErrorString(QObject::tr("Le fichier n'existe plus.\n%1").arg(_path));
		return false;
	}
	if(!fic.open(QIODevice::ReadOnly))
	{
		setErrorString(QObject::tr("Le fichier est protégé en lecture.\n%1").arg(_path));
		return false;
	}
	if(!temp.open())
	{
		fic.close();
		setErrorString(QObject::tr("Impossible de créer un fichier temporaire"));
		return false;
	}

	if(!isOne(_type) && _type != PcSlot)
	{
		quint8 i;
		SaveData *save;

		QByteArray data = header(&fic, newType, !saveAs.isEmpty());

		data.append("MC", 2);//MC
		data.append(125, '\0');
		data.append('\x0E');//xor byte

		fic.seek(start + 128);

		for(i=0 ; i<15 ; ++i)
		{
			save = saves.at(i);

			if(save->hasMCHeader())
			{
				data.append(save->saveMCHeader());//128
				fic.seek(fic.pos() + 128);
			}
			else
			{
				data.append(fic.read(128));//Main header
			}
		}

		data.append(fic.read(6144));//Padding (8192-16*128)

		for(i=0 ; i<15 ; ++i)
		{
			save = saves.at(i);

//			compare(fic.peek(FF8SAVE_SIZE), save->save());
			data.append(save->save());
		}

		if(newType == Vmp) {
			// Rehash
			data = CryptographicHash::hashVmp(data);
		}

		temp.write(data);
	}
	else
	{
		setErrorString(QObject::tr("Type non supporté pour la sauvegarde.\n%1").arg(_type));
		return false;
	}

	fic.close();

#ifndef Q_OS_WINRT
	bool readdPath = false;
	if(fileWatcher.files().contains(path))
	{
		readdPath = true;
		fileWatcher.removePath(path);
	}
#endif

	if(QFile::exists(path) && !QFile::remove(path))
	{
		temp.close();
		setErrorString(QObject::tr("Impossible de supprimer le fichier !\n%1\nÉchec de la sauvegarde.\nVérifiez que le fichier n'est pas utilisé par un autre programme.").arg(path));
#ifndef Q_OS_WINRT
		if(readdPath)	fileWatcher.addPath(path);
#endif
		return false;
	}
	if(!temp.copy(path))
	{
		setErrorString(QObject::tr("Échec de la sauvegarde."));
	}
#ifndef Q_OS_WINRT
	if(readdPath)	fileWatcher.addPath(path);
#endif

	return true;
}

bool SavecardData::saveOne(const SaveData *save, const QString &saveAs, Type newType)
{
	setErrorString(QString());

	const QString path = saveAs.isEmpty() ? _path : saveAs;

	QTemporaryFile temp;
	if(!temp.open()) {
		setErrorString(QObject::tr("Impossible de créer un fichier temporaire"));
		return false;
	}

#ifndef Q_OS_WINRT
	bool readdPath = false;
	if(fileWatcher.files().contains(path)) {
		readdPath = true;
		fileWatcher.removePath(path);
	}
#endif
	UserDirectory userDirectory;
	quint8 slot=0, num=0;

	if(newType == Pc) {
		// Rerelease 2013

		QString filename = path.mid(path.lastIndexOf('/') + 1);
		QRegExp regExp("slot([12])_save(\\d\\d).ff8");

		if(regExp.exactMatch(filename)) {
			QString dirname = path.left(path.lastIndexOf('/'));
			userDirectory.setDirname(dirname);

			if(userDirectory.hasMetadata() && userDirectory.openMetadata()) {
				QStringList capturedTexts = regExp.capturedTexts();
				slot = quint8(capturedTexts.at(1).toInt());
				num = quint8(capturedTexts.at(2).toInt());
			} else if(!userDirectory.hasMetadata()) {
				setErrorString(QObject::tr("Le fichier 'metadata.xml' n'a pas été trouvé dans le dossier '%1'.\n"
										   "Essayez de signer vos sauvegardes manuellement (Fichier > Signer les sauv. pour le Cloud).").arg(dirname));
			} else {
				setErrorString(QObject::tr("Le fichier 'metadata.xml' n'a pas pu être ouvert.\n%1").arg(userDirectory.errorString()));
			}
		}

		if(save->isDelete()) {
			QFile::remove(path);

			// Rerelease 2013: removing from metadata file
			if(slot > 0) {
				userDirectory.updateMetadata(slot, num);
				if(!userDirectory.saveMetadata()) {
					setErrorString(QObject::tr("Le fichier 'metadata.xml' n'a pas pu être mis à jour.\n%1").arg(userDirectory.errorString()));
				}
			}

			return true;
		}
	}

	QByteArray result;

	if(newType == PcUncompressed) {
		result = save->save();
	} else if(newType == Psv) {
		if(!save->hasMCHeader()) {
			setErrorString(QObject::tr("Pas de MC Header défini."));
			return false;
		}
		QByteArray result;
		result.append("\0VSP\0\0\0\0", 8);
		if(!_hashSeed.isEmpty()) {
			result.append(_hashSeed.leftJustified(20, '\0', true));
			result.append(QByteArray(20, '\0')); // Hash
		} else {
			result.append(QByteArray(40, '\0')); // Seed + hash
		}
		result.append("\x00\x00\x00\x00\x00\x00\x00\x00\x14\x00\x00\x00"
					  "\x01\x00\x00\x00\x00\x20\x00\x00\x84\x00\x00\x00"
					  "\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
					  "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x20\x00\x00"
					  "\x03\x90\x00\x00", 52); // unknown (type 1 = PS1 or 2 = PS2 at offset 12)
		result.append(save->MCHeader().mid(10, 32).leftJustified(32, '\0')); // Country + prod code + identifier
		result.append(save->save());
		temp.write(CryptographicHash::hashPsv(result));
	} else {
		result = LZS::compress(save->save());
		int size = result.size();
		result.prepend((char *)&size, 4);
		if(newType == Switch) {
			// Header with size again
			size += 4;
			result.prepend((char *)&size, 4);
			// Padding at the end
			if (result.size() < _switchSaveSize) {
				result.append(_switchSaveSize - result.size(), '\0');
			}
		}
	}

	if(newType == Pc) {
		// Rerelease 2013: updating signature in metadata file
		if(slot > 0) {
			userDirectory.updateMetadata(slot, num, result);
			if(!userDirectory.saveMetadata()) {
				setErrorString(QObject::tr("Le fichier 'metadata.xml' n'a pas pu être mis à jour.\n%1").arg(userDirectory.errorString()));
			}
		}
	}

	temp.write(result);

	if(QFile::exists(path) && !QFile::remove(path))
	{
		setErrorString(QObject::tr("Impossible de supprimer le fichier !\n%1\nÉchec de la sauvegarde.\nEssayez de lancer %2 en tant qu'administrateur.")
				.arg(path).arg(PROG_NAME));
#ifndef Q_OS_WINRT
		if(readdPath)	fileWatcher.addPath(path);
#endif
		return false;
	}
	if(!temp.copy(path))
	{
		setErrorString(QObject::tr("Échec de la sauvegarde."));
	}
#ifndef Q_OS_WINRT
	if(readdPath)	fileWatcher.addPath(path);
#endif

	if(_type == Undefined) {
		setPath(path);
		setType(newType);
	}

	return true;
}

bool SavecardData::save2PS(const QList<int> &ids, const QString &path, const Type newType, const QByteArray &MCHeader)
{
	QTemporaryFile temp;
	quint8 i;
	setErrorString(QString());

	if(!temp.open())
	{
		setErrorString(QObject::tr("Impossible de créer un fichier temporaire"));
		return false;
	}

	QByteArray data;

	data.append(header(nullptr, newType, true));

	data.append("MC", 2);//MC
	data.append(125, '\0');
	data.append('\x0E');//xor byte

	for(i=0 ; i<15 ; ++i)
	{
		// 128 bytes
		if(i >= ids.size())
		{
			data.append(SaveData::emptyMCHeader());
		}
		else
		{
			if(!MCHeader.isEmpty()) {
				QByteArray MCHeaderCpy = MCHeader;
				MCHeaderCpy.replace(26, 2, QString("%1").arg(i, 2, 10, QChar('0')).toLatin1());
				MCHeaderCpy[127] = char(SaveData::xorByte(MCHeaderCpy.constData()));
				data.append(MCHeaderCpy);
			} else {
				data.append(saves.at(ids.at(i))->saveMCHeader());
			}
		}
	}

	data.append(6144, '\0');//Padding

	for(i=0 ; i<15 ; ++i)
	{
		// 8192 bytes
		if(i >= ids.size() || !saves.at(ids.at(i))->isFF8())
		{
			data.append(SAVE_SIZE, '\0');
		}
		else
		{
			data.append(saves.at(ids.at(i))->save());
		}
	}

	if(newType == Vmp) {
		// Rehash
		data = CryptographicHash::hashVmp(data);
	}

	temp.write(data);

#ifndef Q_OS_WINRT
	bool readdPath = false;
	if(fileWatcher.files().contains(path))
	{
		readdPath = true;
		fileWatcher.removePath(path);
	}
#endif

	if(QFile::exists(path) && !QFile::remove(path))
	{
		setErrorString(QObject::tr("Impossible de supprimer le fichier !"
								 "\n%1\nÉchec de la sauvegarde."
								 "\nVérifiez que le fichier n'est pas utilisé"
								 " par un autre programme.").arg(path));
#ifndef Q_OS_WINRT
		if(readdPath)	fileWatcher.addPath(path);
#endif
		return false;
	}
	if(!temp.copy(path))
	{
		setErrorString(QObject::tr("Échec de la sauvegarde."));
	}
#ifndef Q_OS_WINRT
	if(readdPath)	fileWatcher.addPath(path);
#endif

	if(_type == Undefined) {
		setPath(path);
		setType(newType);
		for(i=saves.size() ; i<15 ; ++i) {
			addSave();
		}
		i=0;
		foreach(SaveData *save, saves) {
			if(save->isFF8()) {
				if(!MCHeader.isEmpty()) {
					QByteArray MCHeaderCpy = MCHeader;
					save->setMCHeader(MCHeaderCpy.replace(26, 2, QString("%1").arg(i, 2, 10, QChar('0')).toLatin1()));
				}
			}
			save->setModified(false);
			++i;
		}
	}

	return true;
}

QByteArray SavecardData::header(QFile *srcFile, Type newType, bool saveAs)
{
	if(newType==Vgs)
	{
		// header : 64 bytes
		if(_type==Vgs)
		{
			return srcFile->read(64);
		}
		else
		{
			return QByteArray("\x56\x67\x73\x4d\x01\x00\x00\x00\x01\x00\x00\x00"
							  "\x01\x00\x00\x00\x00\x02", 18)
					.append(QByteArray(46, '\0'));
		}
	}
	else if(newType==Gme)
	{
		QByteArray header;
		// header : 64 bytes
		if(_type==Gme)
		{
			header = srcFile->read(64);
		}
		else
		{
			header = QByteArray("\x31\x32\x33\x2d\x34\x35\x36\x2d\x53\x54\x44\x00"
								"\x00\x00\x00\x00\x00\x00\x01\x00\x01\x4d\x51\xa0"
								"\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0\xa0"
								"\xa0\x00\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00"
								"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
								"\x00\x00\x00\x00", 64);
		}

		// description : 3840 bytes
		if(saveAs)
		{
			return header.append(_description.leftJustified(3840, '\0', true));
		}
		else
		{
			//srcFile->seek(64);
			return header.append(srcFile->read(3840));
		}
	}
	else if(newType==Vmp)
	{
		// header : 128 bytes
		if(_type==Vmp)
		{
			return srcFile->read(128);
		}
		// Empty CRC
		return QByteArray("\x00\x50\x4d\x56\x80", 5)
				.append(QByteArray(123, '\0'));
	}
	else if(newType==_type)
	{
		return srcFile->read(start);
	}
	else
		return QByteArray();
}

bool SavecardData::saveDirectory(const QString &dir)
{
	QString dirname = dir.isEmpty() ? this->dirname() : dir, filePattern;
	bool ok = true;
	int i = 0;

	filePattern = _ff8Installation.saveNamePattern(_slot);

	foreach(const SaveData *save, saves) {
		if(save->isModified()) {
			QString num = QString("%1").arg(i + 1, 2, 10, QChar('0'));
			QString path = filePattern;

			if(!saveOne(save, dirname + path.replace("{num}", num), SavecardData::Pc)) {
				ok = false;
			}
		}
		++i;
	}

	return ok;
}

void SavecardData::compare(quint8 idLeft, quint8 idRight) const
{
	if (qMax(idLeft, idRight) < saves.count()) {
		compare(saves.at(idLeft)->save(), saves.at(idRight)->save());
	} else {
		qDebug() << "Compare overflow";
	}
}

void SavecardData::compare(const QByteArray &oldData, const QByteArray &newData)
{
	qDebug() << "Compare ----";
	for(int i=386 ; i<FF8SAVE_SIZE ; ++i) {
		switch(i) {
		case 0:
			qDebug() << "\tHeader1";break;
		case 386:
			qDebug() << "\tHeader2";break;
		case 464:
			qDebug() << "\tGF";break;
		case 1552:
			qDebug() << "\tPerso";break;
		case 2768:
			qDebug() << "\tBoutiques";break;
		case 3168:
			qDebug() << "\tConfig";break;
		case 3188:
			qDebug() << "\tDivers 1";break;
		case 3220:
			qDebug() << "\tLimit Break";break;
		case 3236:
			qDebug() << "\tObjets";break;
		case 3664:
			qDebug() << "\tStats";break;
		case 3808:
			qDebug() << "\tVar map 1";break;
		case 4064:
			qDebug() << "\tVar map 2";break;
		case 5088:
			qDebug() << "\tMappemonde";break;
		case 5216:
			qDebug() << "\tTriple Triad Cards";break;
		case 5344:
			qDebug() << "\tChocobo";break;
		}

		if(oldData.at(i)!=newData.at(i)) {
			qDebug() << QString("Difference at %1: %2 (%3) | %4 (%5)%6").arg(i).arg((quint8)oldData.at(i),2,16,QChar('0')).arg((quint8)oldData.at(i),8,2,QChar('0')).arg((quint8)newData.at(i),2,16,QChar('0')).arg((quint8)newData.at(i),8,2,QChar('0')).arg(i >= 3808 && i<5088 ? QString(" var%1").arg(i-3808) : "").toLatin1().constData();
		}
	}
}
