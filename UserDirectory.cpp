#include "UserDirectory.h"

UserDirectory::UserDirectory()
{
}

UserDirectory::UserDirectory(const QString &dirname)
{
	setDirname(dirname);
}

bool UserDirectory::isValid() const
{
	return !_userID.isEmpty() && !_metadata.filename().isEmpty();
}

bool UserDirectory::openMetadata()
{
	return _metadata.open();
}

void UserDirectory::updateMetadata(quint8 slot, quint8 num, const QByteArray &saveData)
{
	if(saveData.isEmpty()) {
		_metadata.setTimestamp(slot, num, TIMESTAMP_EMPTY);
	} else if(_metadata.timestamp(slot, num) <= 0) {
		_metadata.setTimestamp(slot, num, QDateTime::currentMSecsSinceEpoch());
	}
	_metadata.updateSignature(slot, num, saveData, _userID);
}

bool UserDirectory::updateSignatures()
{
	if(!openMetadata()) {
		return false;
	}

	for(quint8 slot=1 ; slot<=2 ; ++slot) {
		for(quint8 num=0 ; num<30 ; ++num) {
			QFile f(QString("%1/slot%2_save%3.ff8").arg(_dirname).arg(slot).arg(num, 2, QChar('\0')));
			if(f.open(QIODevice::ReadOnly)) {
				updateMetadata(slot, num, f.readAll());
				f.close();
			} else {
				updateMetadata(slot, num);
			}
		}
	}

	QFile f(QString("%1/chocorpg.ff8").arg(_dirname));
	if(f.open(QIODevice::ReadOnly)) {
		if(_metadata.timestamp() <= 0) {
			_metadata.setTimestamp(QDateTime::currentMSecsSinceEpoch());
		}
		_metadata.updateSignature(f.readAll(), _userID);
		f.close();
	} else {
		_metadata.setTimestamp(TIMESTAMP_EMPTY);
		_metadata.updateSignature(QByteArray(), _userID);
	}

	if(!saveMetadata()) {
		return false;
	}
	return true;
}

bool UserDirectory::saveMetadata()
{
	return _metadata.save();
}

void UserDirectory::setDirname(const QString &dirname)
{
	_dirname = dirname;
	_userID = extractUserID();
	if(!_userID.isEmpty()) {
		QString metadataPath = _dirname + "/metadata.xml";
		if(QFile::exists(metadataPath)) {
			_metadata.setFilename(metadataPath);
		}
	}
}

const QString &UserDirectory::errorString() const
{
	return _metadata.errorString();
}

QString UserDirectory::extractUserID() const
{
	int index = _dirname.lastIndexOf('/', -2);
	QString lastDirname;
	if(index < 0) {
		lastDirname = _dirname;
	} else {
		lastDirname = _dirname.mid(index + 1);
	}
	if(lastDirname.startsWith("user_")) {
		return lastDirname.mid(5);
	}
	return QString();
}
