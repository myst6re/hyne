#include "Metadata.h"

Metadata::Metadata()
{
}

Metadata::Metadata(const QString &filename) :
	_filename(filename)
{
}

bool Metadata::open()
{
	QFile f(_filename);
	if(!f.open(QIODevice::ReadOnly)) {
		setErrorString(f.errorString());
		return false;
	}

	QXmlStreamReader xml(&f);
	bool isInSaveFile = false, ok;
	quint8 num = 0, slot = 0;
	QString type;
	MetadataSaveFile saveFile;

	_hasChocoFile = false;

	while(!xml.atEnd()) {
		switch(xml.readNext()) {

		case QXmlStreamReader::StartElement:

			if(!isInSaveFile && xml.name().compare("savefile", Qt::CaseInsensitive) == 0) {
				isInSaveFile = true;
				saveFile = MetadataSaveFile();
				QXmlStreamAttributes attrs = xml.attributes();
				type = attrs.value("type").toString();
				if(type.compare("ff8", Qt::CaseInsensitive) == 0) {
					num = attrs.value("num").toString().toUInt(&ok);
					if (!ok) {
						setErrorString(QObject::tr("Impossible de charger le contenu du fichier '%1' : attribut 'num' invalide").arg(_filename));
						return false;
					}
					slot = attrs.value("slot").toString().toUInt(&ok);
					if (!ok) {
						setErrorString(QObject::tr("Impossible de charger le contenu du fichier '%1' : attribut 'slot' invalide").arg(_filename));
						return false;
					}
				}
			} else if(isInSaveFile) {
				if(xml.name().compare("timestamp", Qt::CaseInsensitive) == 0) {
					QString timestamp = xml.readElementText();
					if(!timestamp.isEmpty()) {
						saveFile.timestamp = timestamp.toLongLong(&ok);
						if (!ok) {
							saveFile.timestamp = TIMESTAMP_INVALID;
						}
					} else {
						saveFile.timestamp = TIMESTAMP_EMPTY;
					}
				} else if(xml.name().compare("signature", Qt::CaseInsensitive) == 0) {
					saveFile.signature = xml.readElementText();
				}
			}
			break;

		case QXmlStreamReader::EndElement:

			if(isInSaveFile && xml.name().compare("savefile", Qt::CaseInsensitive) == 0) {
				isInSaveFile = false;
				if(type.compare("ff8", Qt::CaseInsensitive) == 0) {
					_saveFiles.insert(SAVE_FILES_KEY(slot, num),
									  saveFile);
				} else if(type.compare("choco", Qt::CaseInsensitive) == 0) {
					_chocoFile = saveFile; // FIXME: several choco elements?
					_hasChocoFile = true;
				}
			}
			break;

		default:
			break;
		}

	}
	if (xml.hasError()) {
		setErrorString(QObject::tr("Impossible de charger le contenu du fichier '%1'").arg(_filename));
		return false;
	}

	return true;
}

bool Metadata::save()
{
	QFile f(_filename);
	if(!f.open(QIODevice::WriteOnly)) {
		setErrorString(f.errorString());
		return false;
	}

	QXmlStreamWriter xml(&f);
	xml.setAutoFormatting(true);
	xml.setAutoFormattingIndent(METADATA_INDENT);

	xml.writeStartDocument();
	xml.writeStartElement("gamestatus");

	QMapIterator<quint16, MetadataSaveFile> it(_saveFiles);
	while(it.hasNext()) {
		it.next();
		const MetadataSaveFile &saveFile = it.value();

		xml.writeStartElement("savefile");
		QXmlStreamAttributes attrs;
		attrs.append("num", QString::number(GET_NUM(it.key())));
		attrs.append("type", "ff8");
		attrs.append("slot", QString::number(GET_SLOT(it.key())));
		xml.writeAttributes(attrs);

		writeSavefileContents(&xml, saveFile);

		xml.writeEndElement(); // savefile
	}

	if(_hasChocoFile) {
		xml.writeStartElement("savefile");
		QXmlStreamAttributes attrs;
		attrs.append("type", "choco");
		xml.writeAttributes(attrs);

		writeSavefileContents(&xml, _chocoFile);

		xml.writeEndElement(); // savefile
	}

	xml.writeEndDocument();

	return true;
}

void Metadata::writeSavefileContents(QXmlStreamWriter *xml, const MetadataSaveFile &saveFile)
{
	QString timestamp;
	if(saveFile.timestamp > 0) {
		timestamp = QString::number(saveFile.timestamp);
	}
	xml->writeTextElement("timestamp", timestamp);
	xml->writeTextElement("signature", saveFile.signature);
}

const QString &Metadata::filename() const
{
	return _filename;
}

void Metadata::setFilename(const QString &filename)
{
	_filename = filename;
}

QString Metadata::signature(quint8 slot, quint8 num) const
{
	return _saveFiles.value(SAVE_FILES_KEY(slot, num)).signature;
}

void Metadata::updateSignature(quint8 slot, quint8 num, const QByteArray &saveData, const QString &userID)
{
	 _saveFiles[SAVE_FILES_KEY(slot, num)].signature = md5sum(saveData, userID);
}

qint64 Metadata::timestamp(quint8 slot, quint8 num) const
{
	return _saveFiles.value(SAVE_FILES_KEY(slot, num)).timestamp;
}

void Metadata::setTimestamp(quint8 slot, quint8 num, qint64 timestamp)
{
	 _saveFiles[SAVE_FILES_KEY(slot, num)].timestamp = timestamp;
}

QString Metadata::signature() const
{
	return _chocoFile.signature;
}

void Metadata::updateSignature(const QByteArray &saveData, const QString &userID)
{
	if(_hasChocoFile) {
		_chocoFile.signature = md5sum(saveData, userID);
	}
}

qint64 Metadata::timestamp() const
{
	return _hasChocoFile ? _chocoFile.timestamp : TIMESTAMP_INVALID;
}

void Metadata::setTimestamp(qint64 timestamp)
{
	if(_hasChocoFile) {
		_chocoFile.timestamp = timestamp;
	}
}

QString Metadata::md5sum(const QByteArray &lzsData, const QString &userID)
{
	// In FF8 md5sum is computed on save data (LZSed) + userID
	QCryptographicHash md5(QCryptographicHash::Md5);
	md5.addData(lzsData + userID.toLatin1());
	return md5.result().toHex().toLower();
}

MetadataSaveFile Metadata::createSaveFile(quint8 slot, quint8 num)
{
	MetadataSaveFile saveFile = MetadataSaveFile();
	_saveFiles.insert(SAVE_FILES_KEY(slot, num), saveFile);

	return saveFile;
}

MetadataSaveFile Metadata::createSaveFile()
{
	_chocoFile = MetadataSaveFile();

	return _chocoFile;
}

const QString &Metadata::errorString() const
{
	return _lastErrorString;
}

void Metadata::setErrorString(const QString &str)
{
	_lastErrorString = str;
}
