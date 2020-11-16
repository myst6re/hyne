/****************************************************************************
 ** Hyne Final Fantasy VIII Save Editor
 ** Copyright (C) 2013 Arzel Jérôme <myst6re@gmail.com>
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
#include "UserDirectory.h"

UserDirectory::UserDirectory()
{
}

UserDirectory::UserDirectory(const QString &dirname)
{
	setDirname(dirname);
}

UserDirectory::UserDirectory(const QString &metadataPath, const QString &userID) :
	_userID(userID)
{
	_metadata.setFilename(metadataPath);
	_dirname = metadataPath.left(metadataPath.lastIndexOf('/'));
}

bool UserDirectory::isValid() const
{
	return hasUserId() && hasMetadata();
}

bool UserDirectory::hasMetadata() const
{
	return !_metadata.filename().isEmpty();
}

bool UserDirectory::hasUserId() const
{
	return !_userID.isEmpty();
}

bool UserDirectory::openMetadata()
{
	return _metadata.open();
}

void UserDirectory::updateMetadata(quint8 slot, quint8 num, const QByteArray &saveData)
{
	if (saveData.isEmpty()) {
		_metadata.setTimestamp(slot, num, TIMESTAMP_EMPTY);
	} else if (_metadata.timestamp(slot, num) <= 0) {
		_metadata.setTimestamp(slot, num, QDateTime::currentMSecsSinceEpoch());
	}
	_metadata.updateSignature(slot, num, saveData, _userID);
}

bool UserDirectory::updateSignatures()
{
	if (!openMetadata()) {
		return false;
	}

	for (quint8 slot = 1; slot <= 2; ++slot) {
		for (quint8 num = 1; num <= 30; ++num) {
			QFile f(QString("%1/slot%2_save%3.ff8").arg(_dirname).arg(slot).arg(num, 2, 10, QChar('0')));
			if (f.open(QIODevice::ReadOnly)) {
				updateMetadata(slot, num, f.readAll());
				f.close();
			} else if (!f.exists()) {
				updateMetadata(slot, num);
			} else {
				return false;
			}
		}
	}

	QFile f(QString("%1/chocorpg.ff8").arg(_dirname));
	if (f.open(QIODevice::ReadOnly)) {
		if (_metadata.timestamp() <= 0) {
			_metadata.setTimestamp(QDateTime::currentMSecsSinceEpoch());
		}
		_metadata.updateSignature(f.readAll(), _userID);
		f.close();
	} else if (!f.exists()) {
		_metadata.setTimestamp(TIMESTAMP_EMPTY);
		_metadata.updateSignature(QByteArray(), _userID);
	} else {
		return false;
	}

	if (!saveMetadata()) {
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
	_userID = extractUserID(_dirname);
	QString metadataPath = _dirname + "/metadata.xml";
	if (QFile::exists(metadataPath)) {
		_metadata.setFilename(metadataPath);
	}
}

const QString &UserDirectory::errorString() const
{
	return _metadata.errorString();
}

QString UserDirectory::extractUserID(const QString &dirname)
{
	int index = dirname.lastIndexOf('/', -2);
	QString lastDirname;
	if (index < 0) {
		lastDirname = dirname;
	} else {
		lastDirname = dirname.mid(index + 1);
	}
	if (lastDirname.startsWith("user_")) {
		return lastDirname.mid(5);
	}
	return QString();
}
