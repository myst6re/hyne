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
#ifndef USERDIRECTORY_H
#define USERDIRECTORY_H

#include <QtCore>
#include "Metadata.h"

class UserDirectory
{
public:
	UserDirectory();
	explicit UserDirectory(const QString &dirname);
	UserDirectory(const QString &metadataPath, const QString &userID);
	inline const Metadata &metadata() const {
		return _metadata;
	}
	bool isValid() const;
	bool hasMetadata() const;
	bool hasUserId() const;
	bool openMetadata();
	void updateMetadata(quint8 slot, quint8 num, const QByteArray &saveData = QByteArray());
	bool updateSignatures();
	bool saveMetadata();
	void setDirname(const QString &dirname);
	inline const QString &userID() const {
		return _userID;
	}
	const QString &errorString() const;
private:
	static QString extractUserID(const QString &dirname);

	Metadata _metadata;
	QString _dirname;
	QString _userID;
};

#endif // USERDIRECTORY_H
