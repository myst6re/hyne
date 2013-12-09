#include "UserDirectory.h"

UserDirectory::UserDirectory()
{
}

UserDirectory::UserDirectory(const QString &dirname) :
	_dirname(dirname)
{
	_userID = extractUserID();
	if(!_userID.isEmpty()) {
		QString metadataPath = _dirname + "/metadata.xml";
		if(QFile::exists(metadataPath)) {
			_metadata.setFilename(metadataPath);
		}
	}
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
	if(_metadata.timestamp(slot, num) <= 0) {
		_metadata.setTimestamp(slot, num, QDateTime::currentMSecsSinceEpoch());
	}
	_metadata.updateSignature(slot, num, saveData, _userID);
}

bool UserDirectory::saveMetadata()
{
	return _metadata.save();
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
